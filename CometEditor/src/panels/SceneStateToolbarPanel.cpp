#include "CometPCH.h"
#include "SceneStateToolbarPanel.h"

#include "imgui.h"
#include "glm/glm.hpp"

#include "Comet/Panels/PanelRegistry.h"
#include "CometEditorResourceManager.h"

namespace Comet
{

REG_PANEL(SceneStateToolbarPanel)

void SceneStateToolbarPanel::onImGuiRender()
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse;
	ImGui::Begin("##sceneStateToolbar", nullptr, windowFlags);

	float buttonSize = ImGui::GetWindowHeight() - 12.0f;
	const auto& buttonIconTexture = (m_playStopButtonState == PlayStopButtonState::PLAY) ? m_playButtonIconTexture : m_stopButtonIconTexture;

	ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x * 0.5f) - (buttonSize * 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<uint64_t>((buttonIconTexture->getRendererID()))), { buttonSize, buttonSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, 0))
	{
		if (m_playStopButtonState == PlayStopButtonState::PLAY)
		{
			m_playStopButtonState = PlayStopButtonState::STOP;
			m_scenePlayedCallback();
		}
		else
		{
			m_playStopButtonState = PlayStopButtonState::PLAY;
			m_sceneStoppedCallback();
		}
	}
	ImGui::PopStyleColor();

	ImGui::End();
}

void SceneStateToolbarPanel::initTextures()
{
	m_playButtonIconTexture = CometEditorResourceManager::getTexture("PlayButtonIcon");
	m_stopButtonIconTexture = CometEditorResourceManager::getTexture("StopButtonIcon");
}

}
