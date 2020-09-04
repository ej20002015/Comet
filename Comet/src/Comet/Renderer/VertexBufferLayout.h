#pragma once
#include "CometPCH.h"

#include "ShaderDataType.h"

namespace Comet
{

	struct VertexBufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t offset;
		uint32_t size;
		uint32_t componentCount;
		bool normalised;

		VertexBufferElement() = default;
		VertexBufferElement(const std::string& name, ShaderDataType type, uint32_t offset = 0, bool normalised = false)
			: name(name), type(type), offset(offset), size(getShaderDataTypeSize(type)), componentCount(getShaderDataTypeComponentCount(type)), normalised(normalised) {}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() = default;
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements);

		std::vector<VertexBufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<VertexBufferElement>::iterator end() { return m_elements.end(); }
		std::vector<VertexBufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<VertexBufferElement>::const_iterator end() const { return m_elements.end(); }

		const std::vector<VertexBufferElement>& getElements() const { return m_elements; }
		uint32_t getStride() const { return m_stride; }

	private:
		void calculateOffsetAndStride();

	private:
		std::vector<VertexBufferElement> m_elements;
		uint32_t m_stride = 0;
	};
}