#pragma once

#include <Comet.h>

#include "Comet/Panels/PanelManager.h"

#include "imgui.h"

namespace Comet
{

class CometEditorLayer : public Layer
{
public:
	enum class SceneState
	{
		EDIT = 0,
		PLAY
	};

public:
	CometEditorLayer() : Layer("Comet Editor Layer") {}
	~CometEditorLayer() = default;

	void onAttach() override;
	void onDetach() override {};
	void onUpdate(const Timestep ts) override;
	void onImGuiRender() override;
	void onEvent(Event& e) override;

private:
	void initRendererItems();
	void initPanels();
	void initImGuiStyle();
	bool onKeyPressedEvent(KeyPressedEvent& e);
	bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	void newScene();
	void saveScene();
	void openScene();
	void openScene(const std::filesystem::path& path);
	void onScenePlay();
	void onSceneStop();
	void exitApplication();

private:
	static const std::unordered_map<std::string, std::string> SCENE_FILE_FILTER_MAP;

	SceneState m_sceneState = SceneState::EDIT;
	Reference<Scene> m_scene;
	EditorCamera m_editorCamera;
	Buffer m_sceneFileFilter;

	PanelManager m_panelManager;

	//Framebuffer for viewport
	Reference<Framebuffer> m_framebuffer;
	const glm::vec2 m_initialFramebufferSize = { 1280.0f, 720.0f };

	//Guizmo
	int32_t m_guizmoOperation = -1;
	bool m_guizmoOperationChangeLocked = false;
};

}