#include "CometPCH.h"
#include "Buffer.h"

namespace Comet
{
	Buffer::~Buffer()
	{
		if (m_data)
			delete[] m_data;
	}

	Unique<Buffer> Buffer::create(void* data, uint32_t size)
	{
		Unique<Buffer> buffer = createUnique<Buffer>();
		buffer->m_size = size;
		buffer->m_data = new byte[size];
		memcpy(buffer->m_data, data, size);
		return buffer;
	}
}
