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

		componentImGuiRender<UUIDComponent>("UUID Component", ComponentWidget::ImGuiRenderUUIDComponentWidget);
		componentImGuiRender<TagComponent>("Tag Component", ComponentWidget::ImGuiRenderTagComponentWidget);
		componentImGuiRender<TransformComponent>("Transform Component", ComponentWidget::ImGuiRenderTransformComponentWidget);
		componentImGuiRender<CameraComponent>("Camera Component", ComponentWidget::ImGuiRenderCameraComponentWidget);
		componentImGuiRender<SpriteComponent>("Sprite Component", ComponentWidget::ImGuiRenderSpriteComponentWidget);

		ImGui::End();
	}

	template<typename T, typename ComponentUIFunction>
	void EntityPropertiesPanel::componentImGuiRender(const std::string& headerName, ComponentUIFunction componentUIFunction)
	{
		if (ImGui::CollapsingHeader(headerName.c_str(), ImGuiTreeNodeFlags_None))
		{
			T& component = m_entity.getComponent<T>();
			componentUIFunction(component);
		}
	}

}
