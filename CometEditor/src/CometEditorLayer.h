#pragma once

#include <Comet.h>

#include "panels/ContentBrowserPanel.h"

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
		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		void newScene();
		void saveScene();
		void openScene();
		void openScene(const std::filesystem::path& path);

	private:
		Timestep m_ts;
		Reference<Scene> m_scene;
		EditorCamera m_editorCamera;

		//Panels
		SceneHierarchyPanel m_sceneHierarchyPanel;
		EntityPropertiesPanel m_entityPropertiesPanel;
		ContentBrowserPanel m_contentBrowserPanel;

		//Viewport
		Reference<Framebuffer> m_framebuffer;
		glm::vec2 m_viewportSize = { 0.0f, 0.0f };
		glm::vec2 m_viewportPosition = { 0.0f, 0.0f };
		glm::vec2 m_mousePositionRelativeToViewport = { 0.0f, 0.0f };
		bool m_viewportFocused = false, m_viewportHovered = false;
		const glm::vec2 m_initialFramebufferSize = { 1280.0f, 720.0f };

		//Guizmo
		int32_t m_guizmoOperation = -1;
		bool m_guizmoOperationChangeLocked = false;
		float m_translateScaleSnapValue = 0.5f, m_rotationSnapValue = 45.0f;
	};

}