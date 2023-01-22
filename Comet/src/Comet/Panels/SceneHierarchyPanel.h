#pragma once
#include "CometPCH.h"

#include "Comet/Scene/Entity.h"
#include "Comet/Scene/Scene.h"

namespace Comet
{

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const Reference<Scene>& scene)
		: m_scene(scene) {}

	void setScene(const Reference<Scene>& scene) { m_scene = scene; m_selectedEntity = Entity(); }

	void onImGuiRender();

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