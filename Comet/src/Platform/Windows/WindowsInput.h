#pragma once
#include "CometPCH.h"

#include "Comet/Core/Input.h"

namespace Comet
{

	class WindowsInput : public Input
	{
	protected:
		bool i_isKeyPressed(KeyCode keyCode) const override;
		bool i_isMouseButtonPressed(KeyCode keyCode) const override;

		std::pair<float, float> i_getMousePosition() const override;
	};

}