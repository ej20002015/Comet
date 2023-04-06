#include "RenderingDeviceInfoPanel.h"

#include "imgui.h"
#include "Comet/Panels/PanelRegistry.h"
#include "Comet/Renderer/RendererAPI.h"

namespace Comet
{

REG_PANEL(RenderingDeviceInfoPanel)

void RenderingDeviceInfoPanel::onImGuiRender()
{
    ImGui::Begin("Rendering Device Information");

    ImGui::Separator();

    ImGui::Text("Renderer Capabilites");

    ImGui::Separator();

    ImGui::Text("Vendor: %s", RendererAPI::getCapabilities().vendor.c_str());
    ImGui::Text("Renderer: %s", RendererAPI::getCapabilities().renderer.c_str());
    ImGui::Text("Version: %s", RendererAPI::getCapabilities().version.c_str());

    ImGui::Text("");

    ImGui::Text("Max Samples: %d", RendererAPI::getCapabilities().maxSamples);
    ImGui::Text("Max Anisotropy: %f", RendererAPI::getCapabilities().maxAnisotropy);
    ImGui::Text("Max Texture Units: %d", RendererAPI::getCapabilities().maxTextureUnits);

    ImGui::End();
}

}
