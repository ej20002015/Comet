#include "CometPCH.h"
#include "SceneHierarchyPanel.h"

#include "Comet/Scene/Components.h"

#include "imgui.h"

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
				m_scene->createEntity();

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		bool deleteEntity = false;

		const UUIDComponent& UUID = entity.getComponent<UUIDComponent>();
		const uint64_t UUIDNum = UUID;
		const std::string& UUIDString = UUID;

		const std::string& tag = entity.getComponent<TagComponent>();
		
		//All nodes are currently leaves as there is no concept of entity parents
		ImGuiTreeNodeFlags flags = ((entity == m_selectedEntity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf;
		bool nodeOpened = ImGui::TreeNodeEx(reinterpret_cast<void*>(UUIDNum), flags, tag.c_str());

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			m_selectedEntity = entity;

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
	}

}
