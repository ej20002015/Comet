#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Comet
{

	class Input
	{
	public:
		static bool isKeyPressed(KeyCode keyCode);
		static bool isMouseButtonPressed(MouseCode mouseCode);

		static glm::vec2 getMousePosition();
	};

}