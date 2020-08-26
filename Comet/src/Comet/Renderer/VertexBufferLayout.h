#pragma once
#include "CometPCH.h"

namespace Comet
{

	enum class VertexBufferDataType
	{
		None, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t getVertexBufferDataTypeSize(VertexBufferDataType type)
	{
		switch (type)
		{
			case VertexBufferDataType::Float:     return 4;
			case VertexBufferDataType::Float2:    return 2 * 4;
			case VertexBufferDataType::Float3:    return 3 * 4;
			case VertexBufferDataType::Float4:    return 4 * 4;
			case VertexBufferDataType::Mat3:      return 3 * 3 * 4;
			case VertexBufferDataType::Mat4:      return 4 * 4 * 4;
			case VertexBufferDataType::Int:       return 4;
			case VertexBufferDataType::Int2:      return 2 * 4;
			case VertexBufferDataType::Int3:      return 3 * 4;
			case VertexBufferDataType::Int4:      return 4 * 4;
			case VertexBufferDataType::Bool:      return 1;

			default:
				CMT_COMET_ASSERT(false, "Unknown Vertex Buffer Data Type");
				return 0;
		}
	}

	static uint32_t getVertexBufferDataTypeComponentCount(VertexBufferDataType type)
	{
		switch (type)
		{
			case VertexBufferDataType::Float:     return 1;
			case VertexBufferDataType::Float2:    return 2;
			case VertexBufferDataType::Float3:    return 3;
			case VertexBufferDataType::Float4:    return 4;
			case VertexBufferDataType::Mat3:      return 3 * 3;
			case VertexBufferDataType::Mat4:      return 4 * 4;
			case VertexBufferDataType::Int:       return 1;
			case VertexBufferDataType::Int2:      return 2;
			case VertexBufferDataType::Int3:      return 3;
			case VertexBufferDataType::Int4:      return 4;
			case VertexBufferDataType::Bool:      return 1;

			default:
				CMT_COMET_ASSERT(false, "Unknown Vertex Buffer Data Type");
				return 0;
		}
	}

	struct VertexBufferElement
	{
		std::string name;
		VertexBufferDataType type;
		uint32_t offset;
		uint32_t size;
		uint32_t componentCount;
		bool normalised;

		VertexBufferElement() = default;
		VertexBufferElement(const std::string& name, VertexBufferDataType type, uint32_t offset = 0, bool normalised = false)
			: name(name), type(type), offset(offset), size(getVertexBufferDataTypeSize(type)), componentCount(getVertexBufferDataTypeComponentCount(type)), normalised(normalised) {}
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