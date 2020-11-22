#include "CometPCH.h"
#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace Comet
{

	static GLenum getGLInternalTextureFormat(const FramebufferFormat framebufferFormat)
	{
		switch (framebufferFormat)
		{
		case FramebufferFormat::RGBA:       return GL_RGBA8; break;
		case FramebufferFormat::FLOAT16:    return GL_RGBA16F; break;
		default:
			Log::cometError("Unknown texture format");
			CMT_COMET_ASSERT(false);
			return 0;
			break;
		}
	}

	Comet::OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
		: m_specification(specification)
	{
		resize(m_specification.width, m_specification.height, true);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(1, &m_colorAttachmentRendererID);
		glDeleteTextures(1, &m_depthAttachmentRendererID);
	}

	void OpenGLFramebuffer::bind() const
	{
		//If renderering to the target buffer in opengl then we need to bind the screen buffer (at 0)
		if (m_specification.swapChainTarget)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		else
			glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		glViewport(0, 0, m_specification.width, m_specification.height);
	}

	void OpenGLFramebuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_specification.width, m_specification.height);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height, bool forceRecreate)
	{
		if (m_specification.width = width && m_specification.height == height && !forceRecreate)
		{
			Log::cometInfo("No need to resize/recreate framebuffer {0}", m_rendererID);
			return;
		}

		m_specification.width = width;
		m_specification.height = height;

		if (m_rendererID)
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(1, &m_colorAttachmentRendererID);
			glDeleteTextures(1, &m_depthAttachmentRendererID);
		}

		Log::cometInfo("Resizing framebuffer: ({0}, {1}) - samples {2}", width, height, m_specification.samples);

		glGenFramebuffers(1, &m_rendererID);

		//Nvidia drive errors causing issues with DSA on framebuffer object immediately - fixed if I bind the new framebuffer and then unbind it
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		bool multisample = m_specification.samples > 1;
		
		if (multisample)
		{
			//Create color attachment and link to framebuffer
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_colorAttachmentRendererID);
			glTextureStorage2DMultisample(m_colorAttachmentRendererID, m_specification.samples, getGLInternalTextureFormat(m_specification.format), width, height, GL_FALSE);
			glNamedFramebufferTexture(m_rendererID, GL_COLOR_ATTACHMENT0, m_colorAttachmentRendererID, 0);

			//Create depth attachment and link to framebuffer
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_depthAttachmentRendererID);
			glTextureStorage2DMultisample(m_depthAttachmentRendererID, m_specification.samples, GL_DEPTH24_STENCIL8, width, height, GL_FALSE);
			glNamedFramebufferTexture(m_rendererID, GL_DEPTH_STENCIL_ATTACHMENT, m_depthAttachmentRendererID, 0);
		}
		else
		{
			//Create color attachment and link to framebuffer
			glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachmentRendererID);
			glTextureStorage2D(m_colorAttachmentRendererID, 1, getGLInternalTextureFormat(m_specification.format), width, height);
			glNamedFramebufferTexture(m_rendererID, GL_COLOR_ATTACHMENT0, m_colorAttachmentRendererID, 0);

			//Create depth attachment and link to framebuffer
			glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachmentRendererID);
			glTextureStorage2D(m_depthAttachmentRendererID, 1, GL_DEPTH24_STENCIL8, width, height);
			glNamedFramebufferTexture(m_rendererID, GL_DEPTH_STENCIL_ATTACHMENT, m_depthAttachmentRendererID, 0);
		}

		CMT_COMET_ASSERT_MESSAGE(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");
	}

	void OpenGLFramebuffer::bindColorTexture(uint32_t slot) const
	{
		if (m_specification.swapChainTarget)
		{
			Log::cometError("Cannot bind color texture of swap chain target framebuffer");
			CMT_COMET_ASSERT(false);
			return;
		}

		glBindTextureUnit(slot, m_colorAttachmentRendererID);
	}

}
