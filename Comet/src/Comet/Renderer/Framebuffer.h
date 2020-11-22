#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"
#include "RendererAPI.h"

namespace Comet
{

	enum class FramebufferFormat
	{
		RGBA, FLOAT16
	};

	struct FramebufferSpecification
	{
		uint32_t width = 1280;
		uint32_t height = 720;
		glm::vec4 clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		FramebufferFormat format = FramebufferFormat::RGBA;
		uint32_t samples = 1;

		//If true then render to the screen
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		static Reference<Framebuffer> create(const FramebufferSpecification& specification = FramebufferSpecification());

		virtual ~Framebuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;

		virtual void bindColorTexture(uint32_t slot = 0) const = 0;

		virtual RendererID getRendererID() const = 0;
		virtual RendererID getColorAttachmentRendererID() const = 0;
		virtual RendererID getDepthAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;
	};

	class FramebufferPool
	{
	public:
		FramebufferPool(uint32_t maxPoolSize = 32);

		void add(const Reference<Framebuffer>& framebuffer);
		void static addToGlobalPool(const Reference<Framebuffer>& framebuffer);

		const std::vector<Reference<Framebuffer>>& getPool() const { return m_pool; }
		static const std::vector<Reference<Framebuffer>>& getGlobalPool() { return s_instance->getPool(); }
		
	private:
		std::vector<Reference<Framebuffer>> m_pool;
		const uint32_t m_maxPoolSize;

		static Unique<FramebufferPool> s_instance;
	};
}