#include "CometPCH.h"
#include "VertexBufferLayout.h"

namespace Comet
{

	VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
		: m_elements(elements)
	{
		calculateOffsetAndStride();
	}

	void VertexBufferLayout::calculateOffsetAndStride()
	{
		uint32_t offset = 0;
		m_stride = 0;

		for (VertexBufferElement& element : m_elements)
		{
			element.offset = offset;
			offset += element.size;
			m_stride += element.size;
		}
	}

}
