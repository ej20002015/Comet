#pragma once
#include "CometPCH.h"

#include "Comet/Panels/Panel.h"
#include "Comet/Renderer/Texture.h"

namespace Comet
{

class SceneStateToolbarPanel : public Panel
{
public:
	DEF_PANEL()

	enum class PlayStopButtonState
	{
		PLAY = 0,
		STOP
	};

public:
	SceneStateToolbarPanel() { initTextures(); }
	SceneStateToolbarPanel(PlayStopButtonState playStopButtonState) { m_playStopButtonState = playStopButtonState; initTextures(); }

	void onImGuiRender() override;

	void setScenePlayedCallback(const std::function<void()>& scenePlayedCallback) { m_scenePlayedCallback = scenePlayedCallback; }
	void setSceneStoppedCallback(const std::function<void()>& sceneStoppedCallback) { m_sceneStoppedCallback = sceneStoppedCallback; };

private:
	void initTextures();

private:
	PlayStopButtonState m_playStopButtonState = PlayStopButtonState::PLAY;

	Reference<Texture2D> m_playButtonIconTexture, m_stopButtonIconTexture;

	//Function callbacks
	std::function<void()> m_scenePlayedCallback = nullptr;
	std::function<void()> m_sceneStoppedCallback = nullptr;
};

}