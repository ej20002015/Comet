#include "CometPCH.h"
#include "IndexBuffer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Comet
{

    Reference<IndexBuffer> IndexBuffer::create(void* data, uint32_t count, IndexBufferUsage indexBufferUsage)
    {
        CMT_COMET_ASSERT_MESSAGE(data, "Data is null");

        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLIndexBuffer>(data, count);
            break;

        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

    Reference<IndexBuffer> IndexBuffer::create(uint32_t count, IndexBufferUsage indexBufferUsage)
    {
        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLIndexBuffer>(count);
            break;

        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

}
