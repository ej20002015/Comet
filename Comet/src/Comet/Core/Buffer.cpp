#include "CometPCH.h"
#include "Buffer.h"

namespace Comet
{

Buffer::Buffer(const uint32_t size)
	: m_data(new byte[size]), m_size(size)
{
	memset(m_data, 0x00, size);
}

Buffer::Buffer(const void* const data, const uint32_t size)
	: m_data(new byte[size]), m_size(size)
{
	memcpy(m_data, data, size);
}

Buffer::Buffer(const byte value, const uint32_t size)
	: m_data(new byte[size]), m_size(size)
{
	memset(m_data, value, size);
}

Buffer::~Buffer()
{
	if (m_data)
		delete[] m_data;
}

Buffer::Buffer(const Buffer& other)
	: m_data(new byte[other.m_size]), m_size(other.m_size)
{
	memcpy(m_data, other.m_data, other.m_size);
}

Buffer::Buffer(Buffer&& other) noexcept
	: m_data(other.m_data), m_size(other.m_size)
{
	other.m_data = nullptr;
}

Buffer& Buffer::operator=(const Buffer& other)
{
	m_data = new byte[other.m_size];
	m_size = other.m_size;
	memcpy(m_data, other.m_data, other.m_size);

	return *this;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
	m_data = other.m_data;
	m_size = other.m_size;
	other.m_data = nullptr;

	return *this;
}

}
