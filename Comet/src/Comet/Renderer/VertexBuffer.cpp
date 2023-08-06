#include "CometPCH.h"
#include "VertexBuffer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Comet
{

Reference<VertexBuffer> VertexBuffer::create(const void* const data, const uint32_t size, const Usage usage)
{

    CMT_COMET_ASSERT_MESSAGE(data, "Data is null");

    switch (RendererAPI::getCurrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
        return createReference<OpenGLVertexBuffer>(data, size, usage);
        break;

    default:
        CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
        return nullptr;
        break;
    }
}

Reference<VertexBuffer> VertexBuffer::create(const uint32_t size, const Usage usage)
{
    switch (RendererAPI::getCurrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
        return createReference<OpenGLVertexBuffer>(size, usage);
        break;

    default:
        CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
        return nullptr;
        break;
    }
}

}
