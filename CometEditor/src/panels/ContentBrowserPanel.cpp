#include "ContentBrowserPanel.h"

#include "imgui.h"

#include "Comet/Panels/PanelRegistry.h"
#include "Comet/ImGui/ImGuiUtilities.h"
#include "CometEditorResourceManager.h"
#include "glm/glm.hpp"

namespace Comet
{

REG_PANEL(ContentBrowserPanel)

const float ContentBrowserPanel::s_buttonPadding = 8.0f;

ContentBrowserPanel::ContentBrowserPanel()
{
	m_directoryIcon = CometEditorResourceManager::getTexture("DirectoryIcon");
	m_fileIcon= CometEditorResourceManager::getTexture("FileIcon");
	m_backButtonIcon = CometEditorResourceManager::getTexture("BackButtonIcon");
}

void ContentBrowserPanel::onImGuiRender()
{
	float thumbnailSize = glm::pow(2.0f, static_cast<float>(m_thumbnailSizeExponent));

	//Disable scrolling for the main content browser window
	ImGuiWindowFlags mainContentBrowserWindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::Begin("Content Browser", NULL, mainContentBrowserWindowFlags);

	renderHeader();

	auto something = ImGui::GetWindowHeight();

	ImGui::BeginChild("##contentBrowserExplorer", { 0.0f, ImGui::GetWindowHeight() - 100.0f });

	float columnWidth = s_buttonPadding + thumbnailSize;
	float panelWidth = ImGui::GetContentRegionAvail().x;

	ImGuiUtilities::setMinimumWindowSize({ columnWidth + 20.0f, 1.0f });

	uint32_t numberOfColumns = static_cast<uint32_t>(glm::max(panelWidth / columnWidth, 1.0f));

	ImGui::Columns(numberOfColumns, nullptr, false);

	for (const std::filesystem::directory_entry& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		renderDirectoryEntry(directoryEntry, thumbnailSize);

	ImGui::Columns(1);

	ImGui::EndChild();

	renderFooter();

	ImGui::End();
}

void ContentBrowserPanel::renderHeader()
{
	ImGui::BeginChild("##contentBrowserHeader", { 0.0f, 30.0f });

	//Display back button. If at the root asset directory then disable it

	bool disabled = m_currentDirectory == m_assetDirectoryPath;

	if (disabled)
		ImGuiUtilities::pushButtonDisabled();

	if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<uint64_t>((m_backButtonIcon->getRendererID()))), { 25.0f, 25.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, 0) && !disabled)
		m_currentDirectory = m_currentDirectory.parent_path();

	if (disabled)
		ImGuiUtilities::popButtonDisabled();

	ImGui::SameLine();

	//Display working directory
	ImGui::Text(m_currentDirectory.string().c_str());

	ImGuiUtilities::seperator();

	ImGui::EndChild();
}

void ContentBrowserPanel::renderFooter()
{
	//TODO: Place in footer that sticks to the bottom of the content browser window
	ImGui::BeginChild("##contentBrowserFooter", { 0.0f, 30.0f }); // Use avail width/height

	ImGui::Separator();

	ImGuiUtilities::beginPropertyGrid();

	int thumbnailSizeExponentOffset = m_thumbnailSizeExponent - 5;
	if (ImGuiUtilities::propertySlider("Thumbnail Size", thumbnailSizeExponentOffset, "%d", 1, 3))
		m_thumbnailSizeExponent = thumbnailSizeExponentOffset + 5;

	ImGuiUtilities::endPropertyGrid();

	ImGui::EndChild();
}

void ContentBrowserPanel::renderDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, float thumbnailSize)
{
	std::filesystem::path directoryEntryPath = directoryEntry.path();
	std::string directoryEntryFilenameString = directoryEntryPath.filename().string();

	//Push ID to give each image button a unique ID (if this is not present the ID will be taken from the rendererID which could be the same for different buttons)
	ImGui::PushID(directoryEntryFilenameString.c_str());

	Reference<Texture2D> buttonIcon = directoryEntry.is_directory() ? m_directoryIcon : m_fileIcon;

	//Temporarily change ImGui button color style to make button background color transparent
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::ImageButton(reinterpret_cast<void*>(static_cast<uint64_t>((buttonIcon->getRendererID()))), { thumbnailSize, thumbnailSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, 0);
	ImGui::PopStyleColor();

	//Double click behaviour on directory entries
	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		if (directoryEntry.is_directory())
			m_currentDirectory /= directoryEntryPath.filename();
	}

	//Drag and Drop behaviour on directory entries
	if (ImGui::BeginDragDropSource())
	{
		std::string pathString = directoryEntryPath.string();
		const char* pathCString = pathString.c_str();
		ImGui::SetDragDropPayload("ContentBrowserEntryPathCString", pathCString, (strlen(pathCString) + 1) * sizeof(char), ImGuiCond_Once);
		ImGui::EndDragDropSource();
	}

	ImGui::TextWrapped(directoryEntryFilenameString.c_str());

	ImGui::NextColumn();

	ImGui::PopID();
}

}