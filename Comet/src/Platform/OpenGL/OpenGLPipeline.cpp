#include "CometPCH.h"
#include "OpenGLPipeline.h"

#include "glad/glad.h"

namespace Comet
{

	static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
			
			default:
				CMT_COMET_ASSERT_MESSAGE(false, "Unknown Vertex Buffer Data Type");
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
			GLenum openGLType = shaderDataTypeToOpenGLBaseType(element.type);
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
