#include "EntityPropertiesPanel.h"

#include "ComponentWidget.h"
#include "CometEditorResourceManager.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace Comet
{

	EntityPropertiesPanel::EntityPropertiesPanel()
	{
		m_optionsTexture = CometEditorResourceManager::getTexture("MenuIcon");
	}

	void EntityPropertiesPanel::onImGuiRender()
	{
		ImGui::Begin("Entity Properties");

		if (!m_entity)
		{
			ImGui::Text("No Entity Selected");
			ImGui::End();
			return;
		}

		ImGui::Indent(-3.0f);
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("ComponentMenu");
		}
		ImGui::Unindent(-3.0f);

		if (ImGui::BeginPopup("ComponentMenu"))
		{
			if (!m_entity.hasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera Component"))
				{
					m_entity.addComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_entity.hasComponent<SpriteComponent>())
			{
				if (ImGui::MenuItem("Sprite Component"))
				{
					m_entity.addComponent<SpriteComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		ImGui::Separator();

		componentImGuiRender<UUIDComponent>("UUID Component", ComponentWidget::ImGuiRenderUUIDComponentWidget, EntityOptionsFlags::NONE);
		componentImGuiRender<TagComponent>("Tag Component", ComponentWidget::ImGuiRenderTagComponentWidget, EntityOptionsFlags::NONE);
		componentImGuiRender<TransformComponent>("Transform Component", ComponentWidget::ImGuiRenderTransformComponentWidget, EntityOptionsFlags::NONE);
		componentImGuiRender<CameraComponent>("Camera Component", ComponentWidget::ImGuiRenderCameraComponentWidget);
		componentImGuiRender<SpriteComponent>("Sprite Component", ComponentWidget::ImGuiRenderSpriteComponentWidget);

		ImGui::End();
	}

	template<typename T, typename ComponentUIFunction>
	void EntityPropertiesPanel::componentImGuiRender(const std::string& headerName, ComponentUIFunction componentUIFunction, EntityOptionsFlags optionsFlags)
	{

		if (!m_entity.hasComponent<T>())
			return;

		ImGui::PushID(headerName.c_str());

		if (optionsFlags == EntityOptionsFlags::NONE)
		{
			if (ImGui::CollapsingHeader(headerName.c_str(), ImGuiTreeNodeFlags_None))
			{
				T& component = m_entity.getComponent<T>();
				componentUIFunction(component);
			}
		}
		else
		{
			ImVec2 contentRegion = ImGui::GetContentRegionAvail();
			ImGui::Columns(2, "0", false);
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::SetColumnWidth(-1, (static_cast<float>(contentRegion.x) * 1.0f) - lineHeight);
			bool opened = ImGui::CollapsingHeader(headerName.c_str(), ImGuiTreeNodeFlags_None);

			ImGui::NextColumn();
			float buttonColumnLength = ImGui::GetColumnWidth();
			float buttonColumnCurrentOffset = ImGui::GetColumnOffset();
			ImGui::SetColumnOffset(-1, buttonColumnCurrentOffset + buttonColumnLength - lineHeight);

			//TODO: fix alignment
			if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<uint64_t>((m_optionsTexture->getRendererID()))), { lineHeight + 3.0f, lineHeight }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, 0))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (optionsFlags & EntityOptionsFlags::CAN_DELETE_ENTITY)
				{
					if (ImGui::MenuItem("Remove Component"))
						removeComponent = true;
				}

				ImGui::EndPopup();
			}

			ImGui::Columns(1, "1");

			if (opened)
			{
				T& component = m_entity.getComponent<T>();
				componentUIFunction(component);
			}

			if (removeComponent)
				m_entity.removeComponent<T>();
		}

		ImGui::PopID();
	}

}
