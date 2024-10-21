project "CometEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.hpp",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Comet/src",
        "src",
        "%{includeDirectories.spdlog}",
        "%{includeDirectories.ImGui}",
        "%{includeDirectories.GLM}",
        "%{includeDirectories.ImGuizmo}",
        "%{includeDirectories.EnTT}",
        "%{includeDirectories.yaml_cpp}"
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