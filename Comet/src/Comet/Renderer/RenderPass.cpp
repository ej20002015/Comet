#include "CometPCH.h"
#include "RenderPass.h"

#include "Platform/OpenGL/OpenGLRenderPass.h"

namespace Comet
{

    Reference<RenderPass> RenderPass::create(const RenderPassSpecification& specification)
    {
        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLRenderPass>(specification);
            break;

        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

}
