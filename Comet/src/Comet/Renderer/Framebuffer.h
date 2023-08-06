#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"
#include "RendererAPI.h"

namespace Comet
{

class Framebuffer
{
public:

	enum class ColorAttachmentFormat
	{
		RGBA8,
		RGBA16F,
		RGBA32F,
		R32I
	};

	enum class DepthAttachmentFormat
	{
		DEPTH24STENCIL8
	};

	struct ColorAttachmentsSpecification
	{
		ColorAttachmentsSpecification() = default;
		ColorAttachmentsSpecification(const std::initializer_list<ColorAttachmentFormat>& attachments) : attachments(attachments) {}

		std::vector<ColorAttachmentFormat> attachments;
	};

	struct DepthAttachmentSpecification
	{
		DepthAttachmentSpecification() = default;
		DepthAttachmentSpecification(DepthAttachmentFormat attachment) : attachment(attachment) {}

		const DepthAttachmentFormat attachment;
	};

	struct Specification
	{
		uint32_t width = DEFAULT_WIDTH;
		uint32_t height = DEFAULT_HEIGHT;
		glm::vec4 clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		ColorAttachmentsSpecification colorAttachments = { ColorAttachmentFormat::RGBA16F };
		DepthAttachmentSpecification depthAttachment = DepthAttachmentFormat::DEPTH24STENCIL8;
		uint32_t samples = 1;

		//TODO: Change to ratio
		//Indicates whether the framebuffer should be resized whenever the main window is resized
		bool resizeOnWindowResize = true;
	};

public:
	static Reference<Framebuffer> create(const Specification& specification = Specification());

	virtual ~Framebuffer() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	//Called specifically when wanting to resize framebuffers on window resize events
	//Has no affect when m_specification.resizeOnWindowResize = false
	virtual void onWindowResize(const uint32_t width, const uint32_t height) = 0;
	//Directly resize framebuffer - does not care what the value of m_specification.resizeOnWindowResize is
	virtual void resize(const uint32_t width, const uint32_t height, const bool forceRecreate = false) = 0;

	virtual int32_t readColorAttachmentPixel(const uint32_t attachmentIndex, const uint32_t x, const uint32_t y) const = 0;

	//Clear depth buffer and any color attachments to the value specified by m_specification.clearColor
	virtual void clear() = 0;
	virtual void clearColorAttachment(const uint32_t attachmentIndex, const int32_t value) = 0;

	virtual void bindColorTexture(const uint32_t attachmentIndex = 0, const uint32_t slot = 0) const = 0;
	virtual void bindDepthTexture(const uint32_t slot = 0) const = 0;

	virtual RendererID getRendererID() const = 0;

	virtual RendererID getColorAttachmentRendererID(const uint32_t attachmentIndex = 0) const = 0;
	virtual RendererID getDepthAttachmentRendererID() const = 0;

	virtual const Specification& getSpecification() const = 0;
};

class FramebufferPool
{
public:
	FramebufferPool(const uint32_t maxPoolSize = 32);

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