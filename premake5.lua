workspace "Comet"
    architecture "x86_64"
    startproject "CometEditor"

    configurations
    {
        "Debug",
        "Release"
    }

    outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    -- Dictionary to store include directory paths relative from the root Comet folder
    includeDirectories = {}
    includeDirectories["spdlog"] = "%{wks.location}/Comet/Vendor/spdlog/include"
    includeDirectories["GLFW"] = "%{wks.location}/Comet/Vendor/GLFW/include"
    includeDirectories["Glad"] = "%{wks.location}/Comet/Vendor/Glad/include"
    includeDirectories["ImGui"] = "%{wks.location}/Comet/Vendor/ImGui"
    includeDirectories["GLM"] = "%{wks.location}/Comet/Vendor/GLM"
    includeDirectories["shaderc"] = "%{wks.location}/Comet/Vendor/shaderc/include"
    includeDirectories["SPIRVCross"] = "%{wks.location}/Comet/Vendor/SPIRV-Cross/include"
    includeDirectories["stb"] = "%{wks.location}/Comet/Vendor/stb/include"
    includeDirectories["ImGuizmo"] = "%{wks.location}/Comet/Vendor/ImGuizmo"
    includeDirectories["EnTT"] = "%{wks.location}/Comet/Vendor/EnTT/include"
    includeDirectories["yaml_cpp"] = "%{wks.location}/Comet/Vendor/yaml-cpp/include"
    includeDirectories["assimp"] = "%{wks.location}/Comet/Vendor/assimp/include"
    includeDirectories["nethost"] = "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.10/runtimes/win-x64/native"

    libDirectories = {}
    libDirectories["shaderc"] = "%{wks.location}/Comet/Vendor/shaderc/bin"
    libDirectories["SPIRVCross"] = "%{wks.location}/Comet/Vendor/SPIRV-Cross/bin"
    libDirectories["nethost"] = "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.10/runtimes/win-x64/native"

    group "Core"
        include "Comet"
        include "CometScriptEngine"
    group ""

    group "Runtime"
        include "Application"
    group ""

    group "Editor"
        include "CometEditor"
    group ""

    group "CometDependencies"
        include "Comet/Vendor/GLFW"
        include "Comet/Vendor/Glad"
        include "Comet/Vendor/ImGui"
        include "Comet/Vendor/yaml-cpp"
        include "Comet/Vendor/assimp"
    group ""