#include "CometPCH.h"
#include "SceneHierarchyPanel.h"

#include "PanelRegistry.h"
#include "Comet/Scene/Components.h"

#include "imgui.h"

namespace Comet
{

REG_PANEL(SceneHierarchyPanel)

void SceneHierarchyPanel::onImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");

	m_scene->m_registry.view<entt::entity>().each([&](entt::entity entityHandle)
	{
		Entity entity = { m_scene.get(), entityHandle };
		drawEntityNode(entity);
	});

	deselectEntityIfNeeded();

	openMenuIfNeeded();

	ImGui::End();
}

void SceneHierarchyPanel::openMenuIfNeeded()
{
	//Open menu to create new entity
	if (ImGui::BeginPopupContextWindow(0, ImGuiMouseButton_Right, false))
	{
		if (ImGui::MenuItem("Create Entity"))
			m_scene->createEntity();

		ImGui::EndPopup();
	}
}

void SceneHierarchyPanel::deselectEntityIfNeeded()
{
	if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
		m_selectedEntity = Entity::Null;
}

void SceneHierarchyPanel::drawEntityNode(Entity entity)
{
	bool deleteEntity = false;

	const uint64_t UUID = entity.getComponent<UUIDComponent>();
	const std::string& tag = entity.getComponent<TagComponent>();
		
	//All nodes are currently leaves as there is no concept of entity parents
	ImGuiTreeNodeFlags flags = ((entity == m_selectedEntity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf;
	bool nodeOpened = ImGui::TreeNodeEx(reinterpret_cast<void*>(UUID), flags, tag.c_str());

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
			m_selectedEntity = Entity::Null;
	}
}

}
