#include "CometPCH.h"
#include "Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Comet
{

    FramebufferDepthAttachmentSpecification& FramebufferDepthAttachmentSpecification::operator= (const FramebufferDepthAttachmentFormat& attachment)
    {
        this->attachment = attachment;
        return *this;
    }

    Reference<Framebuffer> Framebuffer::create(const FramebufferSpecification& specification)
    {
        switch (RendererAPI::getCurrrentRendererAPIType())
        {
            case RendererAPIType::OPENGL:
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

    FramebufferPool::FramebufferPool(uint32_t maxPoolSize)
        : m_maxPoolSize(maxPoolSize)
    {
        m_pool.reserve(maxPoolSize);
    }

    void FramebufferPool::add(const Reference<Framebuffer>& framebuffer)
    {
        if (m_pool.size() == m_maxPoolSize)
        {
            Log::cometError("Max pool size will be exceeded - Frambuffer not added");
            CMT_COMET_ASSERT(false);
            return;
        }

        m_pool.push_back(framebuffer);
    }

    void FramebufferPool::addToGlobalPool(const Reference<Framebuffer>& framebuffer)
    {
        s_instance->add(framebuffer);
    }

}
