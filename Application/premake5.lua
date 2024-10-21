project "Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Comet/src",
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

        postbuildcommands
        {
            "{COPYFILE} %[%{!libDirectories.nethost}/nethost.dll] %[%{!cfg.targetdir}]"
        }
    
    filter "configurations:Debug"
        defines "CMT_DEBUG"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        defines "CMT_RELEASE"
        optimize "on"
        runtime "Release"