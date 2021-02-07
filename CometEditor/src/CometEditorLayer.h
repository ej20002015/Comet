#pragma once

#include <Comet.h>

#include "panels/SceneHierarchyPanel.h"
#include "panels/EntityPropertiesPanel.h"

#include "imgui.h"

namespace Comet
{

	class CometEditorLayer : public Layer
	{
	public:
		CometEditorLayer() : Layer("Comet Editor Layer") {}
		~CometEditorLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate(Timestep ts) override;
		void onImGuiRender() override;
		void onEvent(Event& e) override;

	private:
		Timestep m_ts;
		Reference<Scene> m_scene;

		//Panels
		SceneHierarchyPanel m_sceneHierarchyPanel;
		EntityPropertiesPanel m_entityPropertiesPanel;

		//OrthographicCamera m_orthographicCamera;
		Reference<Texture2D> m_testTexture;
		Reference<Texture2D> m_textureAtlas;
		Reference<Framebuffer> m_framebuffer;
		ImVec2 m_viewportSize;
		bool m_viewportFocused = false, m_viewportHovered = false;
		Entity m_cameraEntity;

		const glm::vec2 m_initialFramebufferSize = { 1280.0f, 720.0f };
	};

}