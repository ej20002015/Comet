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
		OpenGLVertexBuffer(void* data, uint32_t size, VertexBufferUsage usage = VertexBufferUsage::STATIC);
		OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::DYNAMIC);
		~OpenGLVertexBuffer() override;

		void bind() const override;

		void setData(void* data, uint32_t size, uint32_t offset = 0) override;

		uint32_t getSize() const override { return m_size; }
		RendererID getRendererID() const override { return m_rendererID; }

	private:
		RendererID m_rendererID = 0;
		uint32_t m_size;
		VertexBufferUsage m_usage;
		Unique<Buffer> m_localData;
	};

}