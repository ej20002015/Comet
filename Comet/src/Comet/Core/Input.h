#pragma once
#include "CometPCH.h"

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Comet
{

	class Input
	{
	public:
		Input(const Input&) = delete;

		static void init();

		static bool isKeyPressed(KeyCode keyCode);
		static bool isMouseButtonPressed(MouseCode mouseCode);

		static std::pair<float, float> getMousePosition();
		static float getMouseXPosition() { return getMousePosition().first; }
		static float getMouseYPosition() { return getMousePosition().second; }

	protected:
		Input() = default;

		virtual bool i_isKeyPressed(KeyCode keyCode) const = 0;
		virtual bool i_isMouseButtonPressed(MouseCode mouseCode) const = 0;

		virtual std::pair<float, float> i_getMousePosition() const = 0;

	private:
		static Unique<Input> s_instance;
	};

}