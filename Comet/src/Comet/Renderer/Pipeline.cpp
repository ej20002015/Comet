#include "CometPCH.h"
#include "Pipeline.h"

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLPipeline.h"

namespace Comet
{

    Reference<Pipeline> Pipeline::create(const PipelineSpecification& spec)
    {
        CMT_COMET_ASSERT_MESSAGE(spec.layout.getElements().size(), "Layout is empty");

        switch (RendererAPI::getCurrrentRendererAPIType())
        {
            case RendererAPIType::OPENGL:
                return createReference<OpenGLPipeline>(spec);
                break;

            default:
                CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
                return nullptr;
                break;
        }

    }

}
