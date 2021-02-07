#include "EntityPropertiesPanel.h"

#include "ComponentWidget.h"

namespace Comet
{

	void EntityPropertiesPanel::onImGuiRender()
	{
		ImGui::Begin("Entity Properties");

		if (!m_entity)
		{
			ImGui::Text("No Entity Selected");
			ImGui::End();
			return;
		}

		if (m_entity.hasComponent<UUIDComponent>())
		{
			if (ImGui::CollapsingHeader("UUID Component", ImGuiTreeNodeFlags_None))
			{
				const UUIDComponent& UUID = m_entity.getComponent<UUIDComponent>();
				ComponentWidget::ImGuiRenderUUIDComponentWidget(UUID);
			}
		}

		if (m_entity.hasComponent<TagComponent>())
		{
			if (ImGui::CollapsingHeader("Tag Component", ImGuiTreeNodeFlags_None))
			{
				TagComponent& tagComponent = m_entity.getComponent<TagComponent>();
				ComponentWidget::ImGuiRenderTagComponentWidget(tagComponent);
			}
		}

		if (m_entity.hasComponent<TransformComponent>())
		{
			if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_None))
			{
				TransformComponent& transformComponent = m_entity.getComponent<TransformComponent>();
				ComponentWidget::ImGuiRenderTransformComponentWidget(transformComponent);
			}
		}

		if (m_entity.hasComponent<CameraComponent>())
		{
			if (ImGui::CollapsingHeader("Camera Component", ImGuiTreeNodeFlags_None))
			{
				CameraComponent& cameraComponent = m_entity.getComponent<CameraComponent>();
				ComponentWidget::ImGuiRenderCameraComponentWidget(cameraComponent);
			}
		}


		ImGui::End();
	}

}
