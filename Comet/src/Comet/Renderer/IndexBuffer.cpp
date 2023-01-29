#include "CometPCH.h"
#include "IndexBuffer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Comet
{

Reference<IndexBuffer> IndexBuffer::create(const void* const data, const uint32_t count, const Usage indexBufferUsage)
{
    CMT_COMET_ASSERT_MESSAGE(data, "Data is null");

    switch (RendererAPI::getCurrrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
        return createReference<OpenGLIndexBuffer>(data, count);
        break;

    default:
        CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
        return nullptr;
        break;
    }
}

Reference<IndexBuffer> IndexBuffer::create(const uint32_t count, const Usage indexBufferUsage)
{
    switch (RendererAPI::getCurrrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
        return createReference<OpenGLIndexBuffer>(count);
        break;

    default:
        CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
        return nullptr;
        break;
    }
}

}
