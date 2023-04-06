#pragma once
#include "CometPCH.h"

#include "RendererAPI.h"

namespace Comet
{

class IndexBuffer
{
public:

	enum class Usage
	{
		STATIC, DYNAMIC
	};

public:
	static Reference<IndexBuffer> create(const void* const data, const uint32_t count, const Usage usage = Usage::STATIC);
	static Reference<IndexBuffer> create(const uint32_t count, const Usage usage = Usage::DYNAMIC);

	virtual ~IndexBuffer() = default;

	virtual void bind() const = 0;

	virtual void setData(const void* const data, const uint32_t count, const uint32_t offset = 0) = 0;

	virtual uint32_t getSize() const = 0;
	virtual uint32_t getCount() const = 0;
	virtual RendererID getRendererID() const = 0;
};

}