#include "SceneViewportPanels.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include "Comet/Panels/PanelRegistry.h"
#include "Comet/ImGui/ImGuiUtilities.h"
#include "Comet/Core/Application.h"
#include "Comet/Scene/Components.h"
#include "Comet/Core/Input.h"

namespace Comet
{

REG_PANEL(ViewportPanel)

std::pair<bool, int32_t> ViewportPanel::checkClick(const Framebuffer& framebuffer) const
{
    if (m_isHovered && !ImGuizmo::IsOver() && !Input::isKeyPressed(KeyCode::KEY_LEFT_ALT))
    {
        const int32_t pixelValue = framebuffer.readColorAttachmentPixel(1, static_cast<uint32_t>(m_mousePosRelToViewport.x), static_cast<uint32_t>(m_mousePosRelToViewport.y));
        return { true, pixelValue };
    }

    return { false, -1 };
}

void ViewportPanel::onImGuiRender()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Scene Viewport");

    glm::vec2 windowPos = ImGui::GetWindowPos();
    glm::vec2 cursosPos = ImGui::GetCursorPos();
    glm::vec2 viewportPos = windowPos + cursosPos;

    //Calculate position of mouse in the viewport

    glm::vec2 mousePos = ImGui::GetMousePos();
    m_mousePosRelToViewport = mousePos - viewportPos;

    //Flip y-axis so it matches opengl texture coordinate system
    m_mousePosRelToViewport.y = m_size.y - m_mousePosRelToViewport.y;

    m_isHovered = ImGui::IsWindowHovered();
    
    //Unblock events if viewport is being hovered over (events are allowed to propagate down the stack)
    Application::get().getImGuiLayer().setBlocking(!ImGui::IsWindowHovered());

    m_size = ImGui::GetContentRegionAvail();

    ImGui::Image(reinterpret_cast<void*>(static_cast<uint64_t>((m_textureID))), m_size, { 0.0f, 1.0f }, { 1.0f, 0.0f });

    //Set drag drop payload target for opening scene files
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* dragDropPayload = nullptr;
        if (dragDropPayload = ImGui::AcceptDragDropPayload("ContentBrowserEntryPathCString"))
        {
            const char* directoryEntryPathCString = reinterpret_cast<const char*>(dragDropPayload->Data);
            m_onFilepathDropCallback(std::filesystem::path(directoryEntryPathCString));
        }
        ImGui::EndDragDropTarget();
    }

    //ImGuizmo

    m_guizmoOperationChangeLocked = false;

    if (m_getEntityCallback && (m_selectedEntity = m_getEntityCallback()) && m_guizmoOperation > -1)
    {
        //Editor Camera has a perspective projection
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        //Set the viewport
        ImGuizmo::SetRect(windowPos.x, windowPos.y, m_size.x, m_size.y);

        //Get selected entity transform
        TransformComponent& currentlySelectedEntityTransformComponent = m_selectedEntity.getComponent<TransformComponent>();
        glm::mat4 currentlySelectedEntityTransform = currentlySelectedEntityTransformComponent.getTransform();

        //Determine snap value
        float snap = 0.0f;
        if (Input::isKeyPressed(KeyCode::KEY_LEFT_CONTROL))
            snap = (m_guizmoOperation == ImGuizmo::OPERATION::ROTATE) ? GuizmoOptionsPanel::s_rotationSnapValue : GuizmoOptionsPanel::s_translateScaleSnapValue;

        float snapValues[3] = { snap, snap, snap };

        ImGuizmo::Manipulate(glm::value_ptr(m_viewMatrix), glm::value_ptr(m_projectionMatrix), static_cast<ImGuizmo::OPERATION>(m_guizmoOperation), ImGuizmo::MODE::LOCAL, glm::value_ptr(currentlySelectedEntityTransform), nullptr, (snap) ? snapValues : nullptr);

        //Apply changes if gizmo has been used
        if (ImGuizmo::IsUsing())
        {
            //Lock changing of operation during use
            m_guizmoOperationChangeLocked = true;
            currentlySelectedEntityTransformComponent.setTransform(currentlySelectedEntityTransform);
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

REG_PANEL(GuizmoOptionsPanel)

float GuizmoOptionsPanel::s_translateScaleSnapValue = 0.5f;
float GuizmoOptionsPanel::s_rotationSnapValue = 45.0f;

void GuizmoOptionsPanel::onImGuiRender()
{
    ImGui::Begin("Gizmo Options");

    ImGuiUtilities::beginPropertyGrid();

    ImGuiUtilities::property("Translation and Scale Snap Value", s_translateScaleSnapValue, 0.1f, "%.1f", 0.0f, 100.0f);
    ImGuiUtilities::property("Rotation Snap Value", s_rotationSnapValue, 0.1f, "%.1f", 0.0f, 180.0f);

    ImGuiUtilities::endPropertyGrid();

    ImGui::End();
}

}
