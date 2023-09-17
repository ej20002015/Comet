#include "CometPCH.h"
#include "RenderingContext.h"

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLContext.h"


namespace Comet
{

Unique<RenderingContext> RenderingContext::create(void* const implementationWindowHandle)
{
    CMT_COMET_ASSERT_MESSAGE(implementationWindowHandle, "Window handle is null")

    switch (RendererAPI::getCurrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
        return createUnique<OpenGLContext>(reinterpret_cast<GLFWwindow* const>(implementationWindowHandle));
        break;

    default:
        CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
        return nullptr;
        break;
    }
}

}
