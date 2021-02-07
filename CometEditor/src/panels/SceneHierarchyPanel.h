#pragma once

#include <Comet.h>

#include "imgui.h"

namespace Comet
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Reference<Scene>& scene)
			: m_scene(scene) {}

		void setScene(const Reference<Scene>& scene) { m_scene = scene; }

		void onImGuiRender();

		Entity getSelectedEntity() const { return m_selectedEntity; }

	private:
		void drawEntityNode(Entity entity);

	private:
		Reference<Scene> m_scene = nullptr;
		Entity m_selectedEntity;
	};

}