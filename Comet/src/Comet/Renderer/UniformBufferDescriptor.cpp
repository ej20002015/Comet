#include "CometPCH.h"
#include "UniformBufferDescriptor.h"

namespace Comet
{

	void UniformBufferDescriptor::push(const UniformBufferElementDescriptor& uniformDescriptor)
	{
		m_uniformDescriptors.push_back(uniformDescriptor);
	}

	void UniformBufferDescriptor::reserve(const uint32_t count)
	{
		m_uniformDescriptors.reserve(count);
	}

	void UniformStructDescriptor::push(const UniformDescriptor& uniformDescriptor)
	{
		m_uniformDescriptors.push_back(uniformDescriptor);
	}

	void UniformStructDescriptor::reserve(const uint32_t count)
	{
		m_uniformDescriptors.reserve(count);
	}

}
