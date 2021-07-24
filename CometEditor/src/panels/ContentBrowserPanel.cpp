#include "ContentBrowserPanel.h"

#include "imgui.h"

namespace Comet
{

	void ContentBrowserPanel::onImGuiRender()
	{
		ImGui::Begin("Content Browser");

		//Display working directory
		ImGui::Text(m_currentDirectory.string().c_str());

		//Display back button on all directories deeper than the root asset directory
		if (m_currentDirectory != s_AssetDirectoryPath)
		{
			if (ImGui::Button("<-"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}

		for (const std::filesystem::directory_entry& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			std::filesystem::path directoryEntryPath = directoryEntry.path();
			std::string directoryNameFilenameString = directoryEntryPath.filename().string();
			//Make further directories clickable so you navigate into them
			if (directoryEntry.is_directory())
			{
				if (ImGui::Button(directoryNameFilenameString.c_str()))
				{
					m_currentDirectory = directoryEntryPath;
				}
			}
			else
				ImGui::Text("%s", directoryNameFilenameString.c_str());
		}

		ImGui::End();
	}

}