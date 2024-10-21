project "CometScriptEngine"
    kind "SharedLib"
    language "C#"
    dotnetframework "netcoreapp8"
    architecture "x64"

    targetdir ("%{wks.location}/bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "src/**.cs",
    }
    
    filter "configurations:Debug"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        optimize "on"
        runtime "Release"

require "vstudio"

-- Workaround to fix premake bug with specifying platform in C# projects

local function platformsElement(cfg)
    _p(2,'<Platforms>x64</Platforms>')
end

local function generateRuntimeConfigurationFilesElement(cfg)
    _p(2, '<GenerateRuntimeConfigurationFiles>true</GenerateRuntimeConfigurationFiles>')
end

premake.override(premake.vstudio.cs2005.elements, "projectProperties", function (oldfn, cfg)
    return table.join(oldfn(cfg), {
    platformsElement,
    generateRuntimeConfigurationFilesElement,
    })
end)