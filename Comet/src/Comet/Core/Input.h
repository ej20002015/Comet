#pragma once
#include "CometPCH.h"

namespace Comet
{

	//TEMPORARY
	enum class KeyCode : uint32_t
	{
		None = 0
	};

	inline std::ostream& operator <<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int>(keyCode);
		return os;
	}

	class Input
	{
	public:
		Input(const Input&) = delete;

		static void init();

		static bool isKeyPressed(KeyCode keyCode) { return s_instance->i_isKeyPressed(keyCode); }
		static bool isMouseButtonPressed(KeyCode keyCode) { return s_instance->i_isMouseButtonPressed(keyCode); }

		static std::pair<float, float> getMousePosition() {	return s_instance->i_getMousePosition(); }
		static float getMouseXPosition() { return getMousePosition().first; }
		static float getMouseYPosition() { return getMousePosition().second; }

	protected:
		Input() = default;

		virtual bool i_isKeyPressed(KeyCode keyCode) const = 0;
		virtual bool i_isMouseButtonPressed(KeyCode keyCode) const = 0;

		virtual std::pair<float, float> i_getMousePosition() const = 0;

	private:
		static std::unique_ptr<Input> s_instance;
	};

}