#pragma once

#include <iostream>

#include <cstdint>

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