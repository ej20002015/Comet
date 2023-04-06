#pragma once
#include "CometPCH.h"

#include "Panel.h"
#include "Comet/Scene/Entity.h"
#include "Comet/Scene/Scene.h"

namespace Comet
{

class SceneHierarchyPanel : public Panel
{
public:
	DEF_PANEL()

	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const Reference<Scene>& scene)
		: m_scene(scene) {}

	void setScene(const Reference<Scene>& scene) { m_scene = scene; m_selectedEntity = Entity::Null; }

	void onImGuiRender() override;

	void openMenuIfNeeded();

	void deselectEntityIfNeeded();

	Entity getSelectedEntity() const { return m_selectedEntity; }
	void setSelectedEntity(const Entity entity) { m_selectedEntity = entity; }

private:
	void drawEntityNode(Entity entity);

private:
	Reference<Scene> m_scene = nullptr;
	Entity m_selectedEntity;
};

}