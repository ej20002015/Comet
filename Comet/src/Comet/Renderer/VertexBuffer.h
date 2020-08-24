#pragma once
#include "CometPCH.h"

#include "RendererAPI.h"

namespace Comet
{

	enum class VertexBufferUsage
	{
		STATIC, DYNAMIC
	};

	class VertexBuffer
	{
	public:
		static Reference<VertexBuffer> create(void* data, uint32_t size, VertexBufferUsage usage = VertexBufferUsage::STATIC);
		static Reference<VertexBuffer> create(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::DYNAMIC);

		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;

		virtual void setData(void* data, uint32_t size, uint32_t offset = 0) = 0;

		virtual uint32_t getSize() const = 0;
		virtual RendererID getRendererID() const = 0;
	};

}