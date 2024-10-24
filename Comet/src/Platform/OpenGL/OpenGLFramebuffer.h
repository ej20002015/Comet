#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/Framebuffer.h"

namespace Comet
{

class OpenGLFramebuffer : public Framebuffer
{
public:
	OpenGLFramebuffer(const Specification& specification);
	~OpenGLFramebuffer() override;

	void bind() const override;
	void unbind() const override;

	void onWindowResize(const uint32_t width, const uint32_t height) override;
	void resize(const uint32_t width, const uint32_t height, bool forceRecreate = false) override;

	int32_t readColorAttachmentPixel(const uint32_t attachmentIndex, const uint32_t x, const uint32_t y) const override;

	void clear() override;
	void clearColorAttachment(const uint32_t attachmentIndex, const int32_t value) override;

	void bindColorTexture(const uint32_t attachmentIndex = 0, const uint32_t slot = 0) const override;
	void bindDepthTexture(const uint32_t slot = 0) const override;

	RendererID getRendererID() const override { return m_rendererID; };

	RendererID getColorAttachmentRendererID(const uint32_t attachmentIndex = 0) const override { return m_colorAttachmentsRendererID[attachmentIndex]; }
	RendererID getDepthAttachmentRendererID() const override { return m_depthAttachmentRendererID; }

	const Specification& getSpecification() const override { return m_specification; }

private:
	void deleteTexturesAndFramebuffer();
	uint32_t getColorAttachmentCount() const { return static_cast<uint32_t>(m_specification.colorAttachments.attachments.size()); }
	bool isMultisampling() const { return m_specification.samples > 1; }
	void createColorAttachments();
	void createDepthAttachment();
	void specifyDrawBuffers();
	void validateColorAttachmentIndex(const uint32_t index) const;

private:
	RendererID m_rendererID = 0;
	std::vector<RendererID> m_colorAttachmentsRendererID;
	RendererID m_depthAttachmentRendererID = 0;

	Specification m_specification;
};

}