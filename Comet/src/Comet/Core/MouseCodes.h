#pragma once
#include "CometPCH.h"

namespace Comet
{

	enum class MouseCode : uint16_t
	{
		//Taken from glfw3.h

		MOUSE_BUTTON_1      =   0,
		MOUSE_BUTTON_2      =   1,
		MOUSE_BUTTON_3      =   2,
		MOUSE_BUTTON_4      =   3,
		MOUSE_BUTTON_5      =   4,
		MOUSE_BUTTON_6      =   5,
		MOUSE_BUTTON_7      =   6,
		MOUSE_BUTTON_8      =   7,
		MOUSE_BUTTON_LEFT   =   MOUSE_BUTTON_1,
		MOUSE_BUTTON_RIGHT  =   MOUSE_BUTTON_2,
		MOUSE_BUTTON_MIDDLE =   MOUSE_BUTTON_3
	};

	inline std::ostream& operator <<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int>(mouseCode);
		return os;
	}

}