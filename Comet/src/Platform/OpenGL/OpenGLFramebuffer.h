#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/Framebuffer.h"

namespace Comet
{

	//TODO: Create getters for the color and depth attachments' formats

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		void bind() const override;
		void unbind() const override;

		void onWindowResize(uint32_t width, uint32_t height) override;
		void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

		int32_t readColorAttachmentPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) const override;

		void clear() override;
		virtual void clearColorAttachment(uint32_t attachmentIndex, int32_t value) override;

		void bindColorTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override;
		void bindDepthTexture(uint32_t slot = 0) const override;

		RendererID getRendererID() const override { return m_rendererID; };

		RendererID getColorAttachmentRendererID(uint32_t attachmentIndex = 0) const override { return m_colorAttachmentsRendererID[attachmentIndex]; }
		RendererID getDepthAttachmentRendererID() const override { return m_depthAttachmentRendererID; }

		const FramebufferSpecification& getSpecification() const override { return m_specification; }

	private:
		RendererID m_rendererID = 0;
		std::vector<RendererID> m_colorAttachmentsRendererID;
		RendererID m_depthAttachmentRendererID = 0;

		FramebufferSpecification m_specification;
	};

}