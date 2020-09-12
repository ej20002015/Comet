#pragma once
#include "CometPCH.h"

#include "UniformBufferDescriptor.h"
#include "RendererAPI.h"

namespace Comet
{

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void setData(void* data, uint32_t size, uint32_t offset = 0) = 0;

		virtual RendererID getRendererID() const = 0;
		virtual const UniformBufferDescriptor& getDescriptor() const = 0;
	};

	class UniformResource
	{
	public:
		virtual ~UniformResource() = default;

		virtual const UniformResourceDescriptor& getDescriptor() const = 0;
	};

	class UniformStruct
	{
	public:
		virtual ~UniformStruct() = default;

		virtual const UniformStructDescriptor& getDescriptor() const = 0;
	};

}