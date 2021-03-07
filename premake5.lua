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

libDirectories = {}
libDirectories["shaderc"] = "%{wks.location}/Comet/Vendor/shaderc/bin"
libDirectories["SPIRVCross"] = "%{wks.location}/Comet/Vendor/SPIRV-Cross/bin"

include "Comet"
include "Application"
include "CometEditor"

-- Dependencies virtual folder for Comet
group "CometDependencies"
	include "Comet/Vendor/GLFW"
    include "Comet/Vendor/Glad"
    include "Comet/Vendor/ImGui"
group ""

-- Dependencies folder for Comet and client applications (no such dependencies at the moment)

group "CometAndClientDependencies"
group ""