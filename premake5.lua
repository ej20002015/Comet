workspace "Comet"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release"
    }

outputDirectory = "%{cfg.build}-%{cfg.system}-%{cfg.architecture}"

project "Comet"
    location "Comet"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    include
    {
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CMT_PLATFORM_WINDOWS"
            "CMT_BUILD_DLL"
        }
