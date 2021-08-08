#include "ContentBrowserPanel.h"

#include "imgui.h"

#include "Comet/ImGui/ImGuiUtilities.h"
#include "CometEditorResourceManager.h"
#include "glm/glm.hpp"

namespace Comet
{

	const float ContentBrowserPanel::s_buttonPadding = 8.0f;

	ContentBrowserPanel::ContentBrowserPanel()
	{
		m_directoryIcon = CometEditorResourceManager::getTexture("DirectoryIcon");
		m_fileIcon= CometEditorResourceManager::getTexture("FileIcon");
	}

	void ContentBrowserPanel::onImGuiRender()
	{
		float thumbnailSize = glm::pow(2.0f, static_cast<float>(m_thumbnailSizeExponent));

		ImGui::Begin("Content Browser");

		//Display working directory
		ImGui::Text(m_currentDirectory.string().c_str());

		//Display back button on all directories deeper than the root asset directory
		if (m_currentDirectory != m_AssetDirectoryPath)
		{
			if (ImGui::Button("<-"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}

		float columnWidth = s_buttonPadding + thumbnailSize;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		//Prevent numberOfColumns being set to 0
		ImGuiUtilities::setMinimumWindowSize({ columnWidth + 20.0f, 1.0f });

		uint32_t numberOfColumns = static_cast<uint32_t>(glm::max(panelWidth / columnWidth, 1.0f));

		ImGui::Columns(numberOfColumns, nullptr, false);

		for (const std::filesystem::directory_entry& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
			renderDirectoryEntry(directoryEntry, thumbnailSize);

		ImGui::Columns(1);

		//TODO: Place in footer that sticks to the bottom of the content browser window
		int thumbnailSizeExponentOffset = m_thumbnailSizeExponent - 5;
		if (ImGui::SliderInt("Thumbnail Size", &thumbnailSizeExponentOffset, 1, 4, ""))
		{
			m_thumbnailSizeExponent = thumbnailSizeExponentOffset + 5;
		}

		//ImGui::BeginChild("BottomBar", ImVec2(0, 0), true, 0); // Use avail width/height
		//ImGui::Text("Footer");
		//ImGui::EndChild();

		ImGui::End();
	}

	void ContentBrowserPanel::renderDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, float thumbnailSize)
	{
		std::filesystem::path directoryEntryPath = directoryEntry.path();
		std::string directoryEntryFilenameString = directoryEntryPath.filename().string();

		Reference<Texture2D> buttonIcon = directoryEntry.is_directory() ? m_directoryIcon : m_fileIcon;
		ImGui::ImageButton(reinterpret_cast<void*>(static_cast<uint64_t>((buttonIcon->getRendererID()))), { thumbnailSize, thumbnailSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, 0);

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
				m_currentDirectory = directoryEntryPath;
		}

		ImGui::TextWrapped(directoryEntryFilenameString.c_str());

		ImGui::NextColumn();
	}

}