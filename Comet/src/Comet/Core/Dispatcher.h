#pragma once
#include "CometPCH.h"

namespace Comet
{

	struct Dispatcher
	{
		virtual void invoke() = 0;
	};

}