#include "CometPCH.h"
#include "RenderingContext.h"

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLContext.h"


namespace Comet
{

    Unique<RenderingContext> RenderingContext::create(void* implementationWindowHandle)
    {
        CMT_COMET_ASSERT(implementationWindowHandle, "Window handle is null")

        switch (RendererAPI::getCurrrentRendererAPIType())
        {
            case RendererAPIType::OPENGL:
                return createUnique<OpenGLContext>(static_cast<GLFWwindow*>(implementationWindowHandle));
                break;

            default:
                CMT_COMET_ASSERT(false, "Unknown GraphicsAPI");
                return nullptr;
                break;
        }
    }

}
