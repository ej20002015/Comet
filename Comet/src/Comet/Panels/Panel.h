#pragma once
#include "CometPCH.h"

namespace Comet
{

class Panel
{
public:
	virtual ~Panel() = default;

	virtual void onImGuiRender() = 0;
};

#define DEF_PANEL() static bool s_registered;

}

