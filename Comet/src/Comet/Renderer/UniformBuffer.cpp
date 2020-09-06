#include "CometPCH.h"
#include "UniformBuffer.h"

namespace Comet
{

	void UniformContainer::push(const UniformContainerElement& uniform)
	{
		m_uniforms.push_back(uniform);
	}

	void UniformContainer::reserve(const uint32_t count)
	{
		m_uniforms.reserve(count);
	}

}
