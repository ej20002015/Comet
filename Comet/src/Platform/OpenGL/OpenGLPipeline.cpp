#include "CometPCH.h"
#include "OpenGLPipeline.h"

#include "glad/glad.h"

namespace Comet
{

	static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::FLOAT:    return GL_FLOAT;
			case ShaderDataType::FLOAT2:   return GL_FLOAT;
			case ShaderDataType::FLOAT3:   return GL_FLOAT;
			case ShaderDataType::FLOAT4:   return GL_FLOAT;
			case ShaderDataType::MAT3:     return GL_FLOAT;
			case ShaderDataType::MAT4:     return GL_FLOAT;
			case ShaderDataType::UINT:	   return GL_UNSIGNED_INT;
			case ShaderDataType::INT:      return GL_INT;
			case ShaderDataType::INT2:     return GL_INT;
			case ShaderDataType::INT3:     return GL_INT;
			case ShaderDataType::INT4:     return GL_INT;
			case ShaderDataType::BOOL:     return GL_BOOL;
			
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
			if (openGLType == GL_INT || openGLType == GL_UNSIGNED_INT)
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
