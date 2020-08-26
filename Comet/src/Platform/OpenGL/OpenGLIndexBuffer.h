#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/IndexBuffer.h"

#include "Comet/Core/Buffer.h"

namespace Comet
{

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(void* data, uint32_t count, IndexBufferUsage usage = IndexBufferUsage::STATIC);
		OpenGLIndexBuffer(uint32_t count, IndexBufferUsage usage = IndexBufferUsage::DYNAMIC);
		~OpenGLIndexBuffer() override;

		void bind() const override;

		void setData(void* data, uint32_t count, uint32_t offset = 0) override;

		uint32_t getSize() const override { return m_count * sizeof(uint32_t); }
		uint32_t getCount() const override { return m_count; }
		RendererID getRendererID() const override { return m_rendererID; }

	private:
		RendererID m_rendererID = 0;
		uint32_t m_count;
		IndexBufferUsage m_usage;
		Unique<Buffer> m_localData;
	};

}