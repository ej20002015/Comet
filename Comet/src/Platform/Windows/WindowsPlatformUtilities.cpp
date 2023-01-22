#include "CometPCH.h"
#include "Comet/Core/PlatformUtilities.h"

#include <commdlg.h>

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "Comet/Core/Application.h"

namespace Comet
{

	std::string PlatformUtilities::openFile(const char* const filters)
	{
		OPENFILENAMEA ofn;
		CHAR filepath[256] = { 0 };
		CHAR currentDirectory[256] = { 0 };
		memset(&ofn, 0, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::get().getWindow().getImplementationWindow()));
		ofn.lpstrFile = filepath;
		ofn.nMaxFile = sizeof(filepath);
		ofn.lpstrFilter = filters;
		ofn.nFilterIndex = 1;
		if (GetCurrentDirectoryA(sizeof(currentDirectory), currentDirectory))
			ofn.lpstrInitialDir = currentDirectory;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn))
		{
			return filepath;
		}

		return "";
	}


	std::string PlatformUtilities::saveFile(const char* filters, const std::string_view defaultFileExtension)
	{
		OPENFILENAMEA ofn;
		CHAR filepath[256] = { 0 };
		CHAR currentDirectory[256] = { 0 };
		memset(&ofn, 0, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::get().getWindow().getImplementationWindow()));
		ofn.lpstrFile = filepath;
		ofn.nMaxFile = sizeof(filepath);
		ofn.lpstrFilter = filters;
		ofn.nFilterIndex = 1;
		if (!defaultFileExtension.empty())
			ofn.lpstrDefExt = defaultFileExtension.data();
		if (GetCurrentDirectoryA(sizeof(currentDirectory), currentDirectory))
			ofn.lpstrInitialDir = currentDirectory;
		ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn))
		{
			return filepath;
		}

		return "";
	}
}