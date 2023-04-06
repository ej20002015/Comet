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
	static bool isKeyPressed(const KeyCode keyCode);
	static bool isMouseButtonPressed(const MouseButtonCode mouseCode);

	static glm::vec2 getMousePosition();
};

}