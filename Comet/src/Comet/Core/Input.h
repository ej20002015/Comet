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

		static bool isKeyPressed(KeyCode keyCode) { return s_instance->i_isKeyPressed(keyCode); }
		static bool isMouseButtonPressed(MouseCode mouseCode) { return s_instance->i_isMouseButtonPressed(mouseCode); }

		static std::pair<float, float> getMousePosition() {	return s_instance->i_getMousePosition(); }
		static float getMouseXPosition() { return getMousePosition().first; }
		static float getMouseYPosition() { return getMousePosition().second; }

	protected:
		Input() = default;

		virtual bool i_isKeyPressed(KeyCode keyCode) const = 0;
		virtual bool i_isMouseButtonPressed(MouseCode mouseCode) const = 0;

		virtual std::pair<float, float> i_getMousePosition() const = 0;

	private:
		static std::unique_ptr<Input> s_instance;
	};

}