#pragma once
#include "CometPCH.h"

#include "Comet/Panels/Panel.h"

namespace Comet
{

class RenderingDeviceInfoPanel : public Panel
{
public:
	DEF_PANEL()

	void onImGuiRender() override;

};

}