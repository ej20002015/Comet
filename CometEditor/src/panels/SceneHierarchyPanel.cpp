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

		//Deselect an entity
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
			m_selectedEntity = {};

		//Open menu to create new entity
		if (ImGui::BeginPopupContextWindow(0, ImGuiMouseButton_Right, false))
		{
			if (ImGui::MenuItem("Create Entity"))
			{
				m_scene->createEntity();
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		const UUIDComponent& UUID = entity.getComponent<UUIDComponent>();
		const uint64_t UUIDNum = UUID;
		const std::string& UUIDString = UUID;

		const std::string& tag = entity.getComponent<TagComponent>();
		
		ImGui::PushID(reinterpret_cast<void*>(UUIDNum));

		ImGuiTreeNodeFlags flags = ((entity == m_selectedEntity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
		bool nodeOpened = ImGui::TreeNodeEx(reinterpret_cast<void*>("TreeNode"), flags, tag.c_str());

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(UUIDString.c_str());
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			m_selectedEntity = entity;

		//if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		//	ImGui::OpenPopup("DeleteEntity");

		bool deleteEntity = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				deleteEntity = true;

			ImGui::EndPopup();
		}

		if (nodeOpened)
			ImGui::TreePop();

		if (deleteEntity)
		{
			m_scene->deleteEntity(entity);
			if (m_selectedEntity == entity)
				m_selectedEntity = {};
		}

		ImGui::PopID();
	}

}
