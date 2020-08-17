#include "CometPCH.h"
#include "Input.h"

#include "Platform/Windows/WindowsInput.h"

namespace Comet
{

	std::unique_ptr<Input> Input::s_instance = nullptr;

	void Input::init()
	{
	#ifdef CMT_PLATFORM_WINDOWS
			Input::s_instance = std::make_unique<WindowsInput>();
	#else
			CMT_COMET_ASSERT(false, "Unknown platform");
			return nullptr;
	#endif
	}

}