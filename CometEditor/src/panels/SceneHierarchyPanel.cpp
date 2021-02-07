#include "SceneHierarchyPanel.h"

namespace Comet
{

	void SceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_scene->m_registry.each([&](entt::entity entityHandle)
		{
			Entity entity = { m_scene.get(), entityHandle };
			drawEntityNode(entity);
		});

		//Deselect an enitity
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
			m_selectedEntity = {};

		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		const UUIDComponent& UUID = entity.getComponent<UUIDComponent>();
		const uint64_t UUIDNum = UUID;
		const std::string& UUIDString = UUID;

		const std::string& tag = entity.getComponent<TagComponent>();
		
		ImGuiTreeNodeFlags flags = ((entity == m_selectedEntity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
		bool nodeOpened = ImGui::TreeNodeEx(reinterpret_cast<void*>(UUIDNum), flags, tag.c_str());

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(UUIDString.c_str());
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked())
			m_selectedEntity = entity;

		if (nodeOpened)
			ImGui::TreePop();
	}

}
