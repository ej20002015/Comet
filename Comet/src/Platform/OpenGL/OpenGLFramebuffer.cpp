#include "CometPCH.h"
#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace Comet
{

static GLenum getGLInternalColorTextureFormat(const Framebuffer::ColorAttachmentFormat framebufferFormat)
{
	switch (framebufferFormat)
	{
	case Framebuffer::ColorAttachmentFormat::RGBA8:              return GL_RGBA8; break;
	case Framebuffer::ColorAttachmentFormat::RGBA16F:            return GL_RGBA16F; break;
	case Framebuffer::ColorAttachmentFormat::RGBA32F:            return GL_RGBA32F; break;
	case Framebuffer::ColorAttachmentFormat::R32I:               return GL_R32I; break;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown framebuffer color attachment/texture format");
		return 0;
		break;
	}
}

static GLenum getGLInternalDepthTextureFormat(const Framebuffer::DepthAttachmentFormat framebufferFormat)
{
	switch (framebufferFormat)
	{
	case Framebuffer::DepthAttachmentFormat::DEPTH24STENCIL8:    return GL_DEPTH24_STENCIL8;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown framebuffer depth attachment/texture format");
		return 0;
		break;
	}
}

static GLenum getGLDepthAttachmentType(const Framebuffer::DepthAttachmentFormat framebufferFormat)
{
	switch (framebufferFormat)
	{
	case Framebuffer::DepthAttachmentFormat::DEPTH24STENCIL8:    return GL_DEPTH_STENCIL_ATTACHMENT;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown framebuffer depth attachment/texture format");
		return 0;
		break;
	}
}

Comet::OpenGLFramebuffer::OpenGLFramebuffer(const Specification& specification)
	: m_specification(specification)
{
	resize(m_specification.width, m_specification.height, true);
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
	deleteTexturesAndFramebuffer();
}

void OpenGLFramebuffer::deleteTexturesAndFramebuffer()
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

void OpenGLFramebuffer::onWindowResize(const uint32_t width, const uint32_t height)
{
	if (m_specification.resizeOnWindowResize)
		resize(width, height);
}

void OpenGLFramebuffer::resize(const uint32_t width, const uint32_t height, const bool forceRecreate)
{
	if (m_specification.width == width && m_specification.height == height && !forceRecreate)
	{
		Log::cometWarn("No need to resize/recreate framebuffer {0}", m_rendererID);
		return;
	}

	CMT_COMET_ASSERT_MESSAGE(width != 0 && height != 0, fmt::format("Cannot set framebuffer {0}'s width or height to 0", m_rendererID));

	m_specification.width = width;
	m_specification.height = height;

	if (m_rendererID)
	{
		deleteTexturesAndFramebuffer();
		m_colorAttachmentsRendererID.clear();
		m_depthAttachmentRendererID = 0;
	}

	Log::cometInfo("Resizing framebuffer: ({0}, {1}) - samples {2}", width, height, m_specification.samples);

	glGenFramebuffers(1, &m_rendererID);

	//Nvidia driver errors causing issues with DSA on framebuffer object immediately - fixed if I bind the new framebuffer and then unbind it
	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_colorAttachmentsRendererID.resize(getColorAttachmentCount());
	
	createColorAttachments();
	createDepthAttachment();
	specifyDrawBuffers();

	CMT_COMET_ASSERT_MESSAGE(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");
}

void OpenGLFramebuffer::createColorAttachments()
{
	//TODO: This shouldn't be a blanket setting for all color attachments?
	glCreateTextures(isMultisampling() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, getColorAttachmentCount(), m_colorAttachmentsRendererID.data());

	for (uint32_t i = 0; i < getColorAttachmentCount(); ++i)
	{
		if (isMultisampling())
			glTextureStorage2DMultisample(m_colorAttachmentsRendererID[i], m_specification.samples, getGLInternalColorTextureFormat(m_specification.colorAttachments.attachments[i]), m_specification.width, m_specification.height, GL_FALSE);
		else
			glTextureStorage2D(m_colorAttachmentsRendererID[i], 1, getGLInternalColorTextureFormat(m_specification.colorAttachments.attachments[i]), m_specification.width, m_specification.height);

		glNamedFramebufferTexture(m_rendererID, GL_COLOR_ATTACHMENT0 + i, m_colorAttachmentsRendererID[i], 0);
	}
}

void OpenGLFramebuffer::createDepthAttachment()
{
	glCreateTextures(isMultisampling() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 1, &m_depthAttachmentRendererID);

	if (isMultisampling())
		glTextureStorage2DMultisample(m_depthAttachmentRendererID, m_specification.samples, getGLInternalDepthTextureFormat(m_specification.depthAttachment.attachment), m_specification.width, m_specification.height, GL_FALSE);
	else
		glTextureStorage2D(m_depthAttachmentRendererID, 1, getGLInternalDepthTextureFormat(m_specification.depthAttachment.attachment), m_specification.width, m_specification.height);

	glNamedFramebufferTexture(m_rendererID, getGLDepthAttachmentType(m_specification.depthAttachment.attachment), m_depthAttachmentRendererID, 0);
}

void OpenGLFramebuffer::specifyDrawBuffers()
{
	if (getColorAttachmentCount() > 0)
	{
		CMT_COMET_ASSERT_MESSAGE(getColorAttachmentCount() < 5, "Only a maximum of 4 color buffers supported at the moment");
		const GLenum drawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glNamedFramebufferDrawBuffers(m_rendererID, getColorAttachmentCount(), drawBuffers);
	}
	else
	{
		const GLenum drawBuffers = GL_NONE;
		glNamedFramebufferDrawBuffers(m_rendererID, 1, &drawBuffers);
	}
}

int32_t OpenGLFramebuffer::readColorAttachmentPixel(const uint32_t attachmentIndex, const uint32_t x, const uint32_t y) const
{
	if (m_specification.colorAttachments.attachments[attachmentIndex] != ColorAttachmentFormat::R32I)
		throw CometException() << "Attempting to read integer from framebuffer color attachment with format that is not R32I";

	validateColorAttachmentIndex(attachmentIndex);

	//glNamedFramebufferReadBuffer does not work here for whatever reason
	bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int32_t pixelValue;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelValue);
	return pixelValue;
}

void OpenGLFramebuffer::validateColorAttachmentIndex(const uint32_t index) const
{
	if (index >= m_specification.colorAttachments.attachments.size())
		throw CometException() << "Attachment index specified does not correspond to a color attachment in the framebuffer";
}

void OpenGLFramebuffer::clear()
{
	bind();

	glClearColor(m_specification.clearColor.r, m_specification.clearColor.g, m_specification.clearColor.b, m_specification.clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLFramebuffer::clearColorAttachment(const uint32_t attachmentIndex, const int32_t value)
{
	if (m_specification.colorAttachments.attachments[attachmentIndex] != ColorAttachmentFormat::R32I)
		throw CometException() << "Attempting to clear framebuffer color attachment to integer that does not have a format of R32I";

	validateColorAttachmentIndex(attachmentIndex);

	glClearTexImage(m_colorAttachmentsRendererID[attachmentIndex], 0, GL_RED_INTEGER, GL_INT, &value);
}

void OpenGLFramebuffer::bindColorTexture(const uint32_t attachmentIndex, const uint32_t slot) const
{
	validateColorAttachmentIndex(attachmentIndex);

	glBindTextureUnit(slot, m_colorAttachmentsRendererID[attachmentIndex]);
}

void OpenGLFramebuffer::bindDepthTexture(const uint32_t slot) const
{
	glBindTextureUnit(slot, m_depthAttachmentRendererID);
}

}
