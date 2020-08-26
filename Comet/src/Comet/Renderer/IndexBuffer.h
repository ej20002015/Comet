#pragma once
#include "CometPCH.h"

#include "RendererAPI.h"

namespace Comet
{

	enum class IndexBufferUsage
	{
		STATIC, DYNAMIC
	};

	class IndexBuffer
	{
	public:
		static Reference<IndexBuffer> create(void* data, uint32_t count, IndexBufferUsage usage = IndexBufferUsage::STATIC);
		static Reference<IndexBuffer> create(uint32_t count, IndexBufferUsage usage = IndexBufferUsage::DYNAMIC);

		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;

		virtual void setData(void* data, uint32_t count, uint32_t offset = 0) = 0;

		virtual uint32_t getSize() const = 0;
		virtual uint32_t getCount() const = 0;
		virtual RendererID getRendererID() const = 0;
	};

}