#pragma once
#include "CometPCH.h"

#include "Comet/Panels/Panel.h"

namespace Comet
{

class SceneSettingsPanel : public Panel
{
public:
	DEF_PANEL()

	void onImGuiRender() override;

	float getExposure() const { return m_exposure;  }

private:
	float m_exposure = 1.0f;
};

}