#pragma once

#include "CometPCH.h"

namespace Comet
{

	//TEMPORARY
	enum class KeyCode
	{
		None = 0

	};

	inline std::ostream& operator <<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int>(keyCode);
		return os;
	}

}