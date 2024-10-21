#include "CometPCH.h"
#include "Scripting.h"

#define NETHOST_USE_AS_STATIC
#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>

// Standard headers
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

namespace Comet
{

// Globals to hold hostfxr exports
hostfxr_initialize_for_dotnet_command_line_fn init_for_cmd_line_fptr;
hostfxr_initialize_for_runtime_config_fn init_for_config_fptr;
hostfxr_get_runtime_delegate_fn get_delegate_fptr;
hostfxr_run_app_fn run_app_fptr;
hostfxr_close_fn close_fptr;

void* get_export(void* h, const char* name)
{
    void* f = ::GetProcAddress((HMODULE)h, name);
    assert(f != nullptr);
    return f;
}

void* load_library(const char_t* path)
{
    HMODULE h = ::LoadLibraryW(path);
    assert(h != nullptr);
    return (void*)h;
}

// Using the nethost library, discover the location of hostfxr and get exports
bool load_hostfxr(const char_t* assembly_path)
{
    get_hostfxr_parameters params{ sizeof(get_hostfxr_parameters), assembly_path, nullptr };
    // Pre-allocate a large buffer for the path to hostfxr
    char_t buffer[MAX_PATH];
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);
    int rc = get_hostfxr_path(buffer, &buffer_size, &params);
    if (rc != 0)
        return false;

    // Load hostfxr and get desired exports
    // NOTE: The .NET Runtime does not support unloading any of its native libraries. Running
    // dlclose/FreeLibrary on any .NET libraries produces undefined behavior.
    void* lib = load_library(buffer);
    init_for_cmd_line_fptr = (hostfxr_initialize_for_dotnet_command_line_fn)get_export(lib, "hostfxr_initialize_for_dotnet_command_line");
    init_for_config_fptr = (hostfxr_initialize_for_runtime_config_fn)get_export(lib, "hostfxr_initialize_for_runtime_config");
    get_delegate_fptr = (hostfxr_get_runtime_delegate_fn)get_export(lib, "hostfxr_get_runtime_delegate");
    run_app_fptr = (hostfxr_run_app_fn)get_export(lib, "hostfxr_run_app");
    close_fptr = (hostfxr_close_fn)get_export(lib, "hostfxr_close");

    return (init_for_config_fptr && get_delegate_fptr && close_fptr);
}

// Load and initialize .NET Core and get desired function pointer for scenario
load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t* config_path)
{
    // Load .NET Core
    void* load_assembly_and_get_function_pointer = nullptr;
    hostfxr_handle cxt = nullptr;
    int rc = init_for_config_fptr(config_path, nullptr, &cxt);
    if (rc != 0 || cxt == nullptr)
    {
        std::cerr << "Init failed: " << std::hex << std::showbase << rc << std::endl;
        close_fptr(cxt);
        return nullptr;
    }

    // Get the load assembly function pointer
    rc = get_delegate_fptr(
        cxt,
        hdt_load_assembly_and_get_function_pointer,
        &load_assembly_and_get_function_pointer);
    if (rc != 0 || load_assembly_and_get_function_pointer == nullptr)
        std::cerr << "Get delegate failed: " << std::hex << std::showbase << rc << std::endl;

    close_fptr(cxt);
    return (load_assembly_and_get_function_pointer_fn)load_assembly_and_get_function_pointer;
}

void initScripting()
{
    //
    // STEP 1: Load HostFxr and get exported hosting functions
    //
    if (!load_hostfxr(nullptr))
    {
        assert(false && "Failure: load_hostfxr()");
    }

    //
    // STEP 2: Initialize and start the .NET Core runtime
    //
    static constexpr const char_t* config_path = L"..\\bin\\Debug-windows-x86_64\\CometScriptEngine\\netcoreapp8\\CometScriptEngine.runtimeconfig.json"; // Hardcoded for now
    load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer = nullptr;
    load_assembly_and_get_function_pointer = get_dotnet_load_assembly(config_path);
    assert(load_assembly_and_get_function_pointer != nullptr && "Failure: get_dotnet_load_assembly()");



    //
    // STEP 3: Load managed assembly and get function pointer to a managed method
    //
    static constexpr const char_t* dotnetlib_path = L"..\\bin\\Debug-windows-x86_64\\CometScriptEngine\\netcoreapp8\\CometScriptEngine.dll"; // Hardcoded for now
    static constexpr const char_t* dotnet_type = L"CometScriptEngine.Main, CometScriptEngine"; // Hardcoded for now
    static constexpr const char_t* dotnet_type_method = L"Hello"; // Hardcoded for now
    static constexpr const char_t* delegate_type_name = L"CometScriptEngine.Main+entryPointDelegate, CometScriptEngine"; // Hardcoded for now
    // <SnippetLoadAndGet>
    // Function pointer to managed delegate
    component_entry_point_fn hello = nullptr;
    int rc = load_assembly_and_get_function_pointer(
        dotnetlib_path,
        dotnet_type,
        dotnet_type_method,
        nullptr/*delegate_type_name*/,
        nullptr,
        (void**)&hello);
    // </SnippetLoadAndGet>
    assert(rc == 0 && hello != nullptr && "Failure: load_assembly_and_get_function_pointer()");

    //
    // STEP 4: Run managed code
    //
    struct lib_args
    {
        const char_t* message;
        int number;
    };
    for (int i = 0; i < 3; ++i)
    {
        // <SnippetCallManaged>
        lib_args args
        {
            L"from host!",
            i
        };

        hello(&args, sizeof(args));
        // </SnippetCallManaged>
    }

    int x = 0;

    //// Function pointer to managed delegate with non-default signature
    //typedef void (CORECLR_DELEGATE_CALLTYPE* custom_entry_point_fn)(lib_args args);
    //custom_entry_point_fn custom = nullptr;
    //lib_args args
    //{
    //    STR("from host!"),
    //    -1
    //};

    //// UnmanagedCallersOnly
    //rc = load_assembly_and_get_function_pointer(
    //    dotnetlib_path.c_str(),
    //    dotnet_type,
    //    STR("CustomEntryPointUnmanagedCallersOnly") /*method_name*/,
    //    UNMANAGEDCALLERSONLY_METHOD,
    //    nullptr,
    //    (void**)&custom);
    //assert(rc == 0 && custom != nullptr && "Failure: load_assembly_and_get_function_pointer()");
    //custom(args);

    //// Custom delegate type
    //rc = load_assembly_and_get_function_pointer(
    //    dotnetlib_path.c_str(),
    //    dotnet_type,
    //    STR("CustomEntryPoint") /*method_name*/,
    //    STR("DotNetLib.Lib+CustomEntryPointDelegate, DotNetLib") /*delegate_type_name*/,
    //    nullptr,
    //    (void**)&custom);
    //assert(rc == 0 && custom != nullptr && "Failure: load_assembly_and_get_function_pointer()");
    //custom(args);

    //return EXIT_SUCCESS;
}

}

