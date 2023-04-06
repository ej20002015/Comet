#include "RendererStatsPanel.h"

#include "imgui.h"

#include "Comet/Panels/PanelRegistry.h"
#include "Comet/Core/Application.h"
#include "Comet/Renderer/Renderer2D.h"

namespace Comet
{

REG_PANEL(RendererStatsPanel)

void RendererStatsPanel::onImGuiRender()
{
    ImGui::Begin("Renderer2D Statistics");

    ImGui::Text("Frametime");

    ImGui::Separator();

    const Timestep ts = Application::get().getTs();
    ImGui::Text("%fms (%.2f fps)", ts.getMilliseconds(), 1.0f / ts.getSeconds());

    ImGui::Separator();

    const auto& stats = Comet::Renderer2D::getRenderer2DStats();

    ImGui::Text("%u quads", stats.quads);
    ImGui::Text("%u vertices", stats.getVertices());
    ImGui::Text("%u triangles", stats.getTriangles());
    ImGui::Text("%u draw calls", stats.drawCalls);

    ImGui::End();
}

}
