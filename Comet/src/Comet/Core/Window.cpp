#include "CometPCH.h"
#include "Window.h"


//Only include in cpp file so there is no cyclical include
#ifdef CMT_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Comet
{

    std::unique_ptr<Window> Window::create(const WindowProperties& properties)
    {
        #ifdef CMT_PLATFORM_WINDOWS
            return std::make_unique<WindowsWindow>(properties);
        #else
            CMT_COMET_ASSERT(false, "Unknown platform");
            return nullptr;
        #endif
    }

}
