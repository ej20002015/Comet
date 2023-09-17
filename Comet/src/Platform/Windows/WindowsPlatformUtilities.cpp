#include "CometPCH.h"
#include "Comet/Core/PlatformUtilities.h"

#include <commdlg.h>

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "Comet/Core/Application.h"

namespace Comet
{

std::string PlatformUtilities::openFile(const Buffer& filters)
{
	OPENFILENAMEA ofn;
	CHAR filepath[256] = { 0 };
	CHAR currentDirectory[256] = { 0 };
	memset(&ofn, 0, sizeof(OPENFILENAMEA));
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::get().getWindow().getImplementationWindow()));
	ofn.lpstrFile = filepath;
	ofn.nMaxFile = sizeof(filepath);
	ofn.lpstrFilter = filters.as<char>();
	ofn.nFilterIndex = 1;
	if (GetCurrentDirectoryA(sizeof(currentDirectory), currentDirectory))
		ofn.lpstrInitialDir = currentDirectory;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn))
		return filepath;

	return "";
}


std::string PlatformUtilities::saveFile(const Buffer& filters, const std::string_view defaultFileExtension)
{
	OPENFILENAMEA ofn;
	CHAR filepath[256] = { 0 };
	CHAR currentDirectory[256] = { 0 };
	memset(&ofn, 0, sizeof(OPENFILENAMEA));
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::get().getWindow().getImplementationWindow()));
	ofn.lpstrFile = filepath;
	ofn.nMaxFile = sizeof(filepath);
	ofn.lpstrFilter = filters.as<char>();
	ofn.nFilterIndex = 1;
	if (!defaultFileExtension.empty())
		ofn.lpstrDefExt = defaultFileExtension.data();
	if (GetCurrentDirectoryA(sizeof(currentDirectory), currentDirectory))
		ofn.lpstrInitialDir = currentDirectory;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileNameA(&ofn))
		return filepath;

	return "";
}

Buffer PlatformUtilities::constructFilter(const std::unordered_map<std::string, std::string> fileType2ExtensionsMap, bool includeAllFiles)
{
	static constexpr char nullTerminator = '\0';
	static constexpr char allFilesStr[] = "All Files (*.*)\0*.*\0";

	std::stringstream ss;
	for (const auto& [fileType, extensions] : fileType2ExtensionsMap)
	{
		ss << fileType << ' ';
		ss << "(" << extensions << ")";
		ss.write(&nullTerminator, 1);
		ss << extensions;
		ss.write(&nullTerminator, 1);
	}

	if (includeAllFiles)
		ss.write(allFilesStr, sizeof(allFilesStr) - 1);

	ss.write(&nullTerminator, 1);

	ss.seekg(0, std::ios::beg);
	ss.seekg(0, std::ios::end);
	const size_t streamSize = ss.tellg();
	ss.seekg(0, std::ios::beg);

	Buffer buffer(static_cast<uint32_t>(streamSize));
	for (uint32_t i = 0; i < streamSize; i++)
		buffer[i] = static_cast<byte>(ss.get());

	return buffer;
}

}