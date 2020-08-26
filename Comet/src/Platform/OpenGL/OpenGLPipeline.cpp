#include "CometPCH.h"
#include "OpenGLPipeline.h"

#include "glad/glad.h"

namespace Comet
{

	static GLenum vertexBufferDataTypeToOpenGLBaseType(VertexBufferDataType type)
	{
		switch (type)
		{
			case VertexBufferDataType::Float:    return GL_FLOAT;
			case VertexBufferDataType::Float2:   return GL_FLOAT;
			case VertexBufferDataType::Float3:   return GL_FLOAT;
			case VertexBufferDataType::Float4:   return GL_FLOAT;
			case VertexBufferDataType::Mat3:     return GL_FLOAT;
			case VertexBufferDataType::Mat4:     return GL_FLOAT;
			case VertexBufferDataType::Int:      return GL_INT;
			case VertexBufferDataType::Int2:     return GL_INT;
			case VertexBufferDataType::Int3:     return GL_INT;
			case VertexBufferDataType::Int4:     return GL_INT;
			case VertexBufferDataType::Bool:     return GL_BOOL;
			
			default:
				CMT_COMET_ASSERT(false, "Unknown Vertex Buffer Data Type");
				return 0;
		}
	}


	OpenGLPipeline::OpenGLPipeline(const PipelineSpecification& spec)
		: m_spec(spec)
	{
		glCreateVertexArrays(1, &m_vertexArrayRendererID);
	}

	OpenGLPipeline::~OpenGLPipeline()
	{
		glDeleteVertexArrays(1, &m_vertexArrayRendererID);
	}

	void OpenGLPipeline::bind()
	{
		glBindVertexArray(m_vertexArrayRendererID);

		uint32_t attributeIndex = 0;
		uint32_t stride = m_spec.layout.getStride();
		for (const auto& element : m_spec.layout)
		{
			GLenum openGLType = vertexBufferDataTypeToOpenGLBaseType(element.type);
			if (openGLType == GL_INT)
			{
				glVertexAttribIPointer(attributeIndex, 
					element.componentCount,
					openGLType,
					stride,
					(const void*)static_cast<intptr_t>(element.offset)
				);
			}
			else
			{
				glVertexAttribPointer(attributeIndex,
					element.componentCount,
					openGLType,
					element.normalised ? GL_TRUE : GL_FALSE,
					stride,
					(const void*)static_cast<intptr_t>(element.offset)
				);
			}

			glEnableVertexAttribArray(attributeIndex);
			attributeIndex++;
		}
	}

}
