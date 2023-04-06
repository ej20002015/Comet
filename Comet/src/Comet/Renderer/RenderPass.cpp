#include "CometPCH.h"
#include "RenderPass.h"

#include "Platform/OpenGL/OpenGLRenderPass.h"

namespace Comet
{

Reference<RenderPass> RenderPass::create(const Specification& specification)
{
    switch (RendererAPI::getCurrrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
        return createReference<OpenGLRenderPass>(specification);
        break;

    default:
        CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
        return nullptr;
        break;
    }
}

}
