#include "CometPCH.h"
#include "UniformBufferDescriptor.h"

namespace Comet
{

void UniformBufferDescriptor::push(const UniformBufferElementDescriptor& uniformElementBufferDescriptor)
{
	m_uniformDescriptors.push_back(uniformElementBufferDescriptor);
}

void UniformBufferDescriptor::reserve(const uint32_t count)
{
	m_uniformDescriptors.reserve(count);
}

void UniformStructDescriptor::push(const UniformElementDescriptor& uniformElementDescriptor)
{
	m_uniformDescriptors.push_back(uniformElementDescriptor);
}

void UniformStructDescriptor::reserve(const uint32_t count)
{
	m_uniformDescriptors.reserve(count);
}

}
