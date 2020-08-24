#include "CometPCH.h"
#include "Input.h"

#include "Platform/Windows/WindowsInput.h"

namespace Comet
{

	Unique<Input> Input::s_instance = nullptr;

	void Input::init()
	{
		#ifdef CMT_PLATFORM_WINDOWS
			Input::s_instance = createUnique<WindowsInput>();
		#else
			CMT_COMET_ASSERT(false, "Unknown platform");
			Input::s_instance = nullptr;
		#endif
	}

	bool Input::isKeyPressed(KeyCode keyCode) 
	{
		CMT_COMET_ASSERT(s_instance, "Need to initialise the input class first by calling init()");
		return s_instance->i_isKeyPressed(keyCode); 
	}

	bool Input::isMouseButtonPressed(MouseCode mouseCode) 
	{
		CMT_COMET_ASSERT(s_instance, "Need to initialise the input class first by calling init()");
		return s_instance->i_isMouseButtonPressed(mouseCode); 
	}

	std::pair<float, float> Input::getMousePosition() 
	{
		CMT_COMET_ASSERT(s_instance, "Need to initialise the input class first by calling init()");
		return s_instance->i_getMousePosition(); 
	}

}