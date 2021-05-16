#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"
#include "RendererAPI.h"

namespace Comet
{

	enum class FramebufferColorAttachmentFormat
	{
		RGBA8,
		RGBA16F,
		RGBA32F,
		R32I
	};

	enum class FramebufferDepthAttachmentFormat
	{
		DEPTH24STENCIL8
	};

	struct FramebufferColorAttachmentsSpecification
	{
		FramebufferColorAttachmentsSpecification() = default;
		FramebufferColorAttachmentsSpecification(const std::initializer_list<FramebufferColorAttachmentFormat>& attachments) : attachments(attachments) {}

		std::vector<FramebufferColorAttachmentFormat> attachments;
	};

	struct FramebufferDepthAttachmentSpecification
	{
		FramebufferDepthAttachmentSpecification() = default;
		FramebufferDepthAttachmentSpecification(FramebufferDepthAttachmentFormat attachment) : attachment(attachment) {}

		FramebufferDepthAttachmentSpecification& operator= (const FramebufferDepthAttachmentFormat& attachment);

		FramebufferDepthAttachmentFormat attachment;
	};

	struct FramebufferSpecification
	{
		uint32_t width = 1280;
		uint32_t height = 720;
		glm::vec4 clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		FramebufferColorAttachmentsSpecification colorAttachments = { FramebufferColorAttachmentFormat::RGBA16F };
		FramebufferDepthAttachmentSpecification depthAttachment = FramebufferDepthAttachmentFormat::DEPTH24STENCIL8;
		uint32_t samples = 1;

		//TODO: Change to ratio
		//Indicates whether the framebuffer should be resized whenever the main window is resized
		bool resizeOnWindowResize = true;
	};

	class Framebuffer
	{
	public:
		static Reference<Framebuffer> create(const FramebufferSpecification& specification = FramebufferSpecification());

		virtual ~Framebuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		//Called specifically when wanting to resize framebuffers on window resize events
		//Has no affect when m_specification.resizeOnWindowResize = false
		virtual void onWindowResize(uint32_t width, uint32_t height) = 0;
		//Directly resize framebuffer - does not care what the value of m_specification.resizeOnWindowResize is
		virtual void resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;

		//Clear depth buffer and any color attachments to the value specified by m_specification.clearColor
		virtual void clear() const = 0;

		virtual void bindColorTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const = 0;
		virtual void bindDepthTexture(uint32_t slot = 0) const = 0;

		virtual RendererID getRendererID() const = 0;

		virtual RendererID getColorAttachmentRendererID(uint32_t attachmentIndex = 0) const = 0;
		virtual RendererID getDepthAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;
	};

	class FramebufferPool
	{
	public:
		FramebufferPool(uint32_t maxPoolSize = 32);

		void add(const Reference<Framebuffer>& framebuffer);
		static void addToGlobalPool(const Reference<Framebuffer>& framebuffer);

		const std::vector<Reference<Framebuffer>>& getPool() const { return m_pool; }
		static const std::vector<Reference<Framebuffer>>& getGlobalPool() { return s_instance->getPool(); }
		
	private:
		std::vector<Reference<Framebuffer>> m_pool;
		const uint32_t m_maxPoolSize;

		static Unique<FramebufferPool> s_instance;
	};
}