#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/IndexBuffer.h"

#include "Comet/Core/Buffer.h"

namespace Comet
{

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(const void* data, const uint32_t count, const Usage usage = Usage::STATIC);
	OpenGLIndexBuffer(const uint32_t count, const Usage usage = Usage::DYNAMIC);
	~OpenGLIndexBuffer() override;

	void bind() const override;

	void setData(const void* const data, const uint32_t count, const uint32_t offset = 0) override;

	uint32_t getSize() const override { return m_count * sizeof(uint32_t); }
	uint32_t getCount() const override { return m_count; }
	RendererID getRendererID() const override { return m_rendererID; }

private:
	RendererID m_rendererID = 0;
	uint32_t m_count;
	Usage m_usage;
	Unique<Buffer> m_localData;
};

}