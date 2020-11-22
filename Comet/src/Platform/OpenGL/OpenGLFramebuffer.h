#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/Framebuffer.h"

namespace Comet
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		void bind() const override;
		void unbind() const override;

		void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

		void bindColorTexture(uint32_t slot) const override;

		RendererID getRendererID() const override { return m_rendererID; };
		RendererID getColorAttachmentRendererID() const override { return m_colorAttachmentRendererID; }
		RendererID getDepthAttachmentRendererID() const override { return m_depthAttachmentRendererID; }

		const FramebufferSpecification& getSpecification() const override { return m_specification; }

	private:
		RendererID m_rendererID = 0, m_colorAttachmentRendererID = 0, m_depthAttachmentRendererID = 0;
		FramebufferSpecification m_specification;
	};

}