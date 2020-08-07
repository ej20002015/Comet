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
includeDirectories["spdlog"] = "Comet/Vendor/spdlog/include/"

project "Comet"
    location "Comet"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Comet/src",
        "%{includeDirectories.spdlog}"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "CMT_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "CMT_DEBUG"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "CMT_RELEASE"
        optimize "on"
        runtime "Release"

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
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Comet/src",
        "%{includeDirectories.spdlog}"
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