#include "CometPCH.h"
#include "VertexBuffer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Comet
{

    Reference<VertexBuffer> VertexBuffer::create(void* data, uint32_t size, VertexBufferUsage usage)
    {

        CMT_COMET_ASSERT(data, "Data is null");

        switch (RendererAPI::getCurrrentRendererAPIType())
        {
            case RendererAPIType::OPENGL:
                return createReference<OpenGLVertexBuffer>(data, size, usage);
                break;

            default:
                CMT_COMET_ASSERT(false, "Unknown GraphicsAPI");
                return nullptr;
                break;
        }
    }

    Reference<VertexBuffer> VertexBuffer::create(uint32_t size, VertexBufferUsage usage)
    {
        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLVertexBuffer>(size, usage);
            break;

        default:
            CMT_COMET_ASSERT(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

}
