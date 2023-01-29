#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/VertexBuffer.h"
#include "Comet/Renderer/RendererAPI.h"

#include "Comet/Core/Buffer.h"

namespace Comet
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(const void* const data, const uint32_t size, const Usage usage = Usage::STATIC);
	OpenGLVertexBuffer(const uint32_t size, const Usage usage = Usage::DYNAMIC);
	~OpenGLVertexBuffer() override;

	void bind() const override;

	void setData(const void* const data, const uint32_t size, const uint32_t offset = 0) override;

	uint32_t getSize() const override { return m_size; }
	RendererID getRendererID() const override { return m_rendererID; }

private:
	RendererID m_rendererID = 0;
	uint32_t m_size;
	Usage m_usage;
	Unique<Buffer> m_localData;
};

}