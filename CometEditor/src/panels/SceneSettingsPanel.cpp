#include "CometPCH.h"
#include "SceneSettingsPanel.h"

#include "Comet/ImGui/ImGuiUtilities.h"
#include "Comet/Panels/PanelRegistry.h"

namespace Comet
{

REG_PANEL(SceneSettingsPanel)

void SceneSettingsPanel::onImGuiRender()
{
	ImGui::Begin("Scene Settings");
	ImGuiUtilities::beginPropertyGrid();

	ImGuiUtilities::property("Exposure", m_exposure, 0.05, "%.2f", 0.0f, 10.0f);

	ImGuiUtilities::endPropertyGrid();
	ImGui::End();
}

}
