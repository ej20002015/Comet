#include "CometPCH.h"
#include "Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Comet
{

Reference<Framebuffer> Framebuffer::create(const Specification& specification)
{
    switch (RendererAPI::getCurrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
        {
            Reference<OpenGLFramebuffer> framebuffer = createReference<OpenGLFramebuffer>(specification);
            FramebufferPool::addToGlobalPool(framebuffer);
            return framebuffer;
            break;
        }
        default:
        {
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }
}

Unique<FramebufferPool> FramebufferPool::s_instance = createUnique<FramebufferPool>();

FramebufferPool::FramebufferPool(const uint32_t maxPoolSize)
    : m_maxPoolSize(maxPoolSize)
{
    m_pool.reserve(maxPoolSize);
}

void FramebufferPool::add(const Reference<Framebuffer>& framebuffer)
{
    CMT_COMET_ASSERT_MESSAGE(m_pool.size() != m_maxPoolSize, "Max pool size will be exceeded");

    m_pool.push_back(framebuffer);
}

void FramebufferPool::addToGlobalPool(const Reference<Framebuffer>& framebuffer)
{
    s_instance->add(framebuffer);
}

}
