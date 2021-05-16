#include "CometPCH.h"
#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace Comet
{

	static GLenum getGLInternalColorTextureFormat(const FramebufferColorAttachmentFormat framebufferFormat)
	{
		switch (framebufferFormat)
		{
		case FramebufferColorAttachmentFormat::RGBA8:              return GL_RGBA8; break;
		case FramebufferColorAttachmentFormat::RGBA16F:            return GL_RGBA16F; break;
		case FramebufferColorAttachmentFormat::RGBA32F:            return GL_RGBA32F; break;
		case FramebufferColorAttachmentFormat::R32I:               return GL_R32I; break;
		default:
			Log::cometError("Unknown framebuffer color attachment/texture format");
			CMT_COMET_ASSERT(false);
			return 0;
			break;
		}
	}

	static GLenum getGLInternalDepthTextureFormat(const FramebufferDepthAttachmentFormat framebufferFormat)
	{
		switch (framebufferFormat)
		{
		case FramebufferDepthAttachmentFormat::DEPTH24STENCIL8:    return GL_DEPTH24_STENCIL8;
		default:
			Log::cometError("Unknown framebuffer depth attachment/texture format");
			CMT_COMET_ASSERT(false);
			return 0;
			break;
		}
	}

	static GLenum getGLDepthAttachmentType(const FramebufferDepthAttachmentFormat framebufferFormat)
	{
		switch (framebufferFormat)
		{
		case FramebufferDepthAttachmentFormat::DEPTH24STENCIL8:    return GL_DEPTH_STENCIL_ATTACHMENT;
		default:
			Log::cometError("Unknown framebuffer depth attachment/texture format");
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
		glDeleteTextures(static_cast<uint32_t>(m_colorAttachmentsRendererID.size()), m_colorAttachmentsRendererID.data());
		glDeleteTextures(1, &m_depthAttachmentRendererID);
	}

	void OpenGLFramebuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_specification.width, m_specification.height);
	}

	void OpenGLFramebuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::onWindowResize(uint32_t width, uint32_t height)
	{
		if (m_specification.resizeOnWindowResize)
			resize(width, height);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height, bool forceRecreate)
	{
		if (m_specification.width == width && m_specification.height == height && !forceRecreate)
		{
			Log::cometWarn("No need to resize/recreate framebuffer {0}", m_rendererID);
			return;
		}

		if (width < 1 || height < 1)
		{
			Log::cometError("Cannot set framebuffer {0}'s width or height to 0 or below", m_rendererID);
			CMT_COMET_ASSERT(false);
			return;
		}

		m_specification.width = width;
		m_specification.height = height;

		if (m_rendererID)
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(static_cast<uint32_t>(m_colorAttachmentsRendererID.size()), m_colorAttachmentsRendererID.data());
			glDeleteTextures(1, &m_depthAttachmentRendererID);

			m_colorAttachmentsRendererID.clear();
			m_depthAttachmentRendererID = 0;
		}

		Log::cometInfo("Resizing framebuffer: ({0}, {1}) - samples {2}", width, height, m_specification.samples);

		glGenFramebuffers(1, &m_rendererID);

		//Nvidia drive errors causing issues with DSA on framebuffer object immediately - fixed if I bind the new framebuffer and then unbind it
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		bool multisample = m_specification.samples > 1;

		//Create color attachments
		uint32_t colorAttachmentCount = static_cast<uint32_t>(m_specification.colorAttachments.attachments.size());
		m_colorAttachmentsRendererID.resize(colorAttachmentCount);

		glCreateTextures(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, colorAttachmentCount, m_colorAttachmentsRendererID.data());

		for (uint32_t i = 0; i < colorAttachmentCount; ++i)
		{
			if (multisample)
				glTextureStorage2DMultisample(m_colorAttachmentsRendererID[i], m_specification.samples, getGLInternalColorTextureFormat(m_specification.colorAttachments.attachments[i]), width, height, GL_FALSE);
			else
				glTextureStorage2D(m_colorAttachmentsRendererID[i], 1, getGLInternalColorTextureFormat(m_specification.colorAttachments.attachments[i]), width, height);

			glNamedFramebufferTexture(m_rendererID, GL_COLOR_ATTACHMENT0 + i, m_colorAttachmentsRendererID[i], 0);
		}

		//Create depth attachment
		glCreateTextures(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 1, &m_depthAttachmentRendererID);

		if (multisample)
			glTextureStorage2DMultisample(m_depthAttachmentRendererID, m_specification.samples, getGLInternalDepthTextureFormat(m_specification.depthAttachment.attachment), width, height, GL_FALSE);
		else
			glTextureStorage2D(m_depthAttachmentRendererID, 1, getGLInternalDepthTextureFormat(m_specification.depthAttachment.attachment), width, height);

		glNamedFramebufferTexture(m_rendererID, getGLDepthAttachmentType(m_specification.depthAttachment.attachment), m_depthAttachmentRendererID, 0);

		CMT_COMET_ASSERT_MESSAGE(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");
	}

	void OpenGLFramebuffer::clear() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_specification.width, m_specification.height);

		glClearColor(m_specification.clearColor.r, m_specification.clearColor.g, m_specification.clearColor.b, m_specification.clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLFramebuffer::bindColorTexture(uint32_t attachmentIndex, uint32_t slot) const
	{
		glBindTextureUnit(slot, m_colorAttachmentsRendererID[attachmentIndex]);
	}

	void OpenGLFramebuffer::bindDepthTexture(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_depthAttachmentRendererID);
	}

}
