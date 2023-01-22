#include "CometPCH.h"
#include "Buffer.h"

namespace Comet
{

Buffer::~Buffer()
{
	if (m_data)
		delete[] m_data;
}

Unique<Buffer> Buffer::create(const uint32_t size)
{
	Unique<Buffer> buffer = createUnique<Buffer>();
	buffer->m_size = size;
	buffer->m_data = new byte[size];
	memset(buffer->m_data, 0, size);
	return buffer;
}

Unique<Buffer> Buffer::create(const void* const data, const uint32_t size)
{
	Unique<Buffer> buffer = createUnique<Buffer>();
	buffer->m_size = size;
	buffer->m_data = new byte[size];
	memcpy(buffer->m_data, data, size);
	return buffer;
}

Unique<Buffer> Buffer::create(const uint32_t value, const uint32_t size)
{
	Unique<Buffer> buffer = createUnique<Buffer>();
	buffer->m_size = size;
	buffer->m_data = new byte[size];
	memset(buffer->m_data, value, size);
	return buffer;
}

}
