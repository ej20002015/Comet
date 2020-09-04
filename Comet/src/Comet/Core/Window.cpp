#include "CometPCH.h"
#include "Window.h"


//Only include in cpp file so there is no cyclical include
#ifdef CMT_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Comet
{

    Unique<Window> Window::create(const WindowProperties& properties)
    {
        #ifdef CMT_PLATFORM_WINDOWS
            return createUnique<WindowsWindow>(properties);
        #else
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown platform");
            return nullptr;
        #endif
    }

}
