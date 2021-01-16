workspace "Comet"
    architecture "x86_64"
    startproject "Application"

    configurations
    {
        "Debug",
        "Release"
    }

outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Dictionary to store include directory paths relative from the root Comet folder
includeDirectories = {}
includeDirectories["spdlog"] = "Comet/Vendor/spdlog/include"
includeDirectories["GLFW"] = "Comet/Vendor/GLFW/include"
includeDirectories["Glad"] = "Comet/Vendor/Glad/include"
includeDirectories["ImGui"] = "Comet/Vendor/ImGui"
includeDirectories["GLM"] = "Comet/Vendor/GLM"
includeDirectories["shaderc"] = "Comet/Vendor/shaderc/include"
includeDirectories["SPIRVCross"] = "Comet/Vendor/SPIRV-Cross/include"
includeDirectories["stb"] = "Comet/Vendor/stb/include"

includeDirectories["EnTT"] = "Comet/Vendor/EnTT/include"

libDirectories = {}
libDirectories["shaderc"] = "Vendor/shaderc/bin"
libDirectories["SPIRVCross"] = "Vendor/SPIRV-Cross/bin"

-- Dependencies virtual folder for Comet
group "CometDependencies"
	include "Comet/Vendor/GLFW"
    include "Comet/Vendor/Glad"
    include "Comet/Vendor/ImGui"
group ""

-- Dependencies folder for Comet and client applications (no such dependencies at the moment)

group "CometAndClientDependencies"
group ""

project "Comet"
    location "Comet"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

    pchheader "CometPCH.h"
    pchsource "Comet/src/CometPCH.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    -- Remove files that are specific to target platform (will be included in the platform filters below)

    removefiles
    {
        "%{prj.name}/src/Comet/Windows"
    }

    includedirs
    {
        "Comet/src",
        "%{includeDirectories.spdlog}",
        "%{includeDirectories.GLFW}",
        "%{includeDirectories.Glad}",
        "%{includeDirectories.ImGui}",
        "%{includeDirectories.shaderc}",
        "%{includeDirectories.SPIRVCross}",
        "%{includeDirectories.GLM}",
        "%{includeDirectories.stb}",
        "%{includeDirectories.EnTT}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui"
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "CMT_PLATFORM_WINDOWS"
        }

        -- Re-include platform specific files that have been removed above

        files
        {
            "%{prj.name}/src/Comet/Windows"
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

project "Application"
    location "Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Comet/src",
        "%{includeDirectories.spdlog}",
        "%{includeDirectories.ImGui}",
        "%{includeDirectories.GLM}",
        "%{includeDirectories.EnTT}"
    }

    filter "system:windows"
        systemversion "latest"

        links
        {
            "Comet"
        }

        defines
        {
            "CMT_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "CMT_DEBUG"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        defines "CMT_RELEASE"
        optimize "on"
        runtime "Release"

project "CometEditor"
    location "CometEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Comet/src",
        "%{includeDirectories.spdlog}",
        "%{includeDirectories.ImGui}",
        "%{includeDirectories.GLM}",
        "%{includeDirectories.EnTT}"
    }

    filter "system:windows"
        systemversion "latest"

        links
        {
            "Comet"
        }

        defines
        {
            "CMT_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "CMT_DEBUG"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        defines "CMT_RELEASE"
        optimize "on"
        runtime "Release"