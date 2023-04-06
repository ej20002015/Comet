#pragma once
#include "CometPCH.h"

namespace Comet
{

class Buffer
{
public:
	Buffer() = default;
	explicit Buffer(const uint32_t size);
	Buffer(const void* const data, const uint32_t size);
	Buffer(const byte value, const uint32_t size);
	~Buffer();

	Buffer(const Buffer& other);
	Buffer(Buffer&& other) noexcept;

	Buffer& operator=(const Buffer& other);
	Buffer& operator=(Buffer&& other) noexcept;

	byte& operator[](const uint32_t index) { return m_data[index]; }
	byte operator[](const uint32_t index) const { return m_data[index]; }

	template<typename T>
	const T* as() const { return reinterpret_cast<T* const>(m_data); }

	uint32_t getSize() const { return m_size; }
	void* const getData() { return reinterpret_cast<void* const>(m_data); }

private:
	byte* m_data = nullptr;
	uint32_t m_size = 0;
};

}