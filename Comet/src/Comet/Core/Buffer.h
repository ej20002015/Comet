#pragma once
#include "CometPCH.h"

namespace Comet
{

class Buffer
{
public:
	Buffer() : m_data(nullptr), m_size(0) {}
	~Buffer();

	static Unique<Buffer> create(const uint32_t size);
	static Unique<Buffer> create(const void* const data, const uint32_t size);
	static Unique<Buffer> create(const uint32_t value, const uint32_t size);

	byte& operator[](const uint32_t index) { return m_data[index]; }
	byte operator[](const uint32_t index) const { return m_data[index]; }

	template<typename T>
	T* const as() { return reinterpret_cast<T* const>(m_data); }

	uint32_t getSize() const { return m_size; }
	void* const getData() { return reinterpret_cast<void* const>(m_data); }

private:
	byte* m_data;
	uint32_t m_size;
};

}