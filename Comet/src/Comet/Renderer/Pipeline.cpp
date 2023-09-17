#include "CometPCH.h"
#include "Pipeline.h"

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLPipeline.h"

namespace Comet
{

Reference<Pipeline> Pipeline::create(const Specification& spec)
{
    CMT_COMET_ASSERT_MESSAGE(spec.layout.getElements().size(), "Layout is empty");

    switch (RendererAPI::getCurrentRendererAPIType())
    {
        case RendererAPI::Type::OPENGL:
            return createReference<OpenGLPipeline>(spec);
            break;

        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
    }
}

}
