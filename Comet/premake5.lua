project "Comet"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDirectory .. "/%{prj.name}")

    pchheader "CometPCH.h"
    pchsource "src/CometPCH.cpp"

    files
    {
        "src/**.h",
        "src/**.hpp",
        "src/**.cpp",

        "Vendor/ImGuizmo/**.h",
        "Vendor/ImGuizmo/**.cpp"
    }

    -- Remove files that are specific to target platform (will be included in the platform filters below)

    removefiles
    {
        "src/Platform/Windows/**.h",
        "src/Platform/Windows/**.cpp"
    }

    includedirs
    {
        "src",
        "%{includeDirectories.spdlog}",
        "%{includeDirectories.GLFW}",
        "%{includeDirectories.Glad}",
        "%{includeDirectories.ImGui}",
        "%{includeDirectories.shaderc}",
        "%{includeDirectories.SPIRVCross}",
        "%{includeDirectories.GLM}",
        "%{includeDirectories.stb}",
        "%{includeDirectories.ImGuizmo}",
        "%{includeDirectories.EnTT}",
        "%{includeDirectories.yaml_cpp}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "yaml-cpp"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "NOMINMAX"
    }

    filter "files:Vendor/ImGuizmo/ImGuizmo.cpp"
	    flags { "NoPCH" }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "CMT_PLATFORM_WINDOWS"
        }

        -- Re-include platform specific files that have been removed above

        files
        {
            "src/Platform/Windows/**.h",
            "src/Platform/Windows/**.cpp"
        }
    
    filter "configurations:Debug"
        defines "CMT_DEBUG"
        symbols "On"
        runtime "Debug"

        links
        {
            "%{libDirectories.shaderc}/Debug/GenericCodeGend.lib",
            "%{libDirectories.shaderc}/Debug/glslangd.lib",
            "%{libDirectories.shaderc}/Debug/HLSLd.lib",
            "%{libDirectories.shaderc}/Debug/MachineIndependentd.lib",
            "%{libDirectories.shaderc}/Debug/OGLCompilerd.lib",
            "%{libDirectories.shaderc}/Debug/OSDependentd.lib",
            "%{libDirectories.shaderc}/Debug/shaderc.lib",
            "%{libDirectories.shaderc}/Debug/shaderc_util.lib",
            "%{libDirectories.shaderc}/Debug/SPIRVd.lib",
            "%{libDirectories.shaderc}/Debug/SPIRV-Tools.lib",
            "%{libDirectories.shaderc}/Debug/SPIRV-Tools-opt.lib",

            "%{libDirectories.SPIRVCross}/Debug/spirv-cross-cd.lib",
            "%{libDirectories.SPIRVCross}/Debug/spirv-cross-cored.lib",
            "%{libDirectories.SPIRVCross}/Debug/spirv-cross-cppd.lib",
            "%{libDirectories.SPIRVCross}/Debug/spirv-cross-glsld.lib",
            "%{libDirectories.SPIRVCross}/Debug/spirv-cross-hlsld.lib",
            "%{libDirectories.SPIRVCross}/Debug/spirv-cross-msld.lib",
            "%{libDirectories.SPIRVCross}/Debug/spirv-cross-reflectd.lib",
            "%{libDirectories.SPIRVCross}/Debug/spirv-cross-utild.lib"
        }

    filter "configurations:Release"
        defines "CMT_RELEASE"
        optimize "on"
        runtime "Release"

        links
        {
            "%{libDirectories.shaderc}/Release/GenericCodeGen.lib",
            "%{libDirectories.shaderc}/Release/glslang.lib",
            "%{libDirectories.shaderc}/Release/HLSL.lib",
            "%{libDirectories.shaderc}/Release/MachineIndependent.lib",
            "%{libDirectories.shaderc}/Release/OGLCompiler.lib",
            "%{libDirectories.shaderc}/Release/OSDependent.lib",
            "%{libDirectories.shaderc}/Release/shaderc.lib",
            "%{libDirectories.shaderc}/Release/shaderc_util.lib",
            "%{libDirectories.shaderc}/Release/SPIRV.lib",
            "%{libDirectories.shaderc}/Release/SPIRV-Tools.lib",
            "%{libDirectories.shaderc}/Release/SPIRV-Tools-opt.lib",

            "%{libDirectories.SPIRVCross}/Release/spirv-cross-c.lib",
            "%{libDirectories.SPIRVCross}/Release/spirv-cross-core.lib",
            "%{libDirectories.SPIRVCross}/Release/spirv-cross-cpp.lib",
            "%{libDirectories.SPIRVCross}/Release/spirv-cross-glsl.lib",
            "%{libDirectories.SPIRVCross}/Release/spirv-cross-hlsl.lib",
            "%{libDirectories.SPIRVCross}/Release/spirv-cross-msl.lib",
            "%{libDirectories.SPIRVCross}/Release/spirv-cross-reflect.lib",
            "%{libDirectories.SPIRVCross}/Release/spirv-cross-util.lib"
        }