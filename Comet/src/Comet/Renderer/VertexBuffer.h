#pragma once
#include "CometPCH.h"

#include "RendererAPI.h"

namespace Comet
{

class VertexBuffer
{
public:

	enum class Usage
	{
		STATIC, DYNAMIC
	};

public:
	static Reference<VertexBuffer> create(const void* const data, const uint32_t size, const Usage usage = Usage::STATIC);
	static Reference<VertexBuffer> create(const uint32_t size, const Usage usage = Usage::DYNAMIC);

	virtual ~VertexBuffer() = default;

	virtual void bind() const = 0;

	virtual void setData(const void* const data, const uint32_t size, const uint32_t offset = 0) = 0;

	virtual uint32_t getSize() const = 0;
	virtual RendererID getRendererID() const = 0;
};

}