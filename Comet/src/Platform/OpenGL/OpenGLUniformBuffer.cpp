#include "CometPCH.h"
#include "OpenGLUniformBuffer.h"

namespace Comet
{

	Comet::OpenGLUniformBuffer::OpenGLUniformBuffer(const UniformBufferDescriptor& descriptor)
		: m_descriptor(descriptor)
	{
		glCreateBuffers(1, &m_rendererID);
		glNamedBufferData(m_rendererID, m_descriptor.getSize(), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_descriptor.getBindingPoint(), m_rendererID);
	}

	Reference<OpenGLUniformBuffer> OpenGLUniformBuffer::create(const UniformBufferDescriptor& uniformBufferDescriptor)
	{
		return createReference<OpenGLUniformBuffer>(uniformBufferDescriptor);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGLUniformBuffer::setData(void* data, uint32_t size, uint32_t offset)
	{
		CMT_COMET_ASSERT_MESSAGE(size <= m_descriptor.getSize() - offset, "Uniform Buffer overflow");

		m_localData = Buffer::create(data, size);

		glNamedBufferSubData(m_rendererID, 0, size, data);
	}

	OpenGLUniformStruct::OpenGLUniformStruct(const UniformStructDescriptor& descriptor, const RendererID programID)
		: m_descriptor(descriptor)
	{
		//Retrieve locations of uniform struct members
		for (UniformDescriptor member : descriptor)
		{
			std::string memberName = getMemberFullName(member.getName());
			m_locations[memberName] = glGetUniformLocation(programID, memberName.c_str());
		}
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, float data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::FLOAT)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'FLOAT' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform1f(m_locations[getMemberFullName(uniformDescriptor.getName())], data);
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, glm::vec2 data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::FLOAT2)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'FLOAT2' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform2f(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y);
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, glm::vec3 data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::FLOAT3)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'FLOAT3' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform3f(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y, data.z);
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, glm::vec4 data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::FLOAT4)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'FLOAT4' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform4f(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y, data.z, data.w);
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, glm::mat3 data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::MAT3)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'MAT3' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform3fv(m_locations[getMemberFullName(uniformDescriptor.getName())], 1, &data[0][0]);
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, glm::mat4 data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::MAT4)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'MAT4' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform4fv(m_locations[getMemberFullName(uniformDescriptor.getName())], 1, &data[0][0]);
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, int32_t data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::INT)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'INT' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform1i(m_locations[getMemberFullName(uniformDescriptor.getName())], data);
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, glm::ivec2 data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::INT2)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'INT2' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform2i(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y);
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, glm::ivec3 data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::INT3)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'INT3' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform3i(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y, data.z);
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, glm::ivec4 data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::INT4)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'INT4' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform4i(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y, data.z, data.w);
	}

	void OpenGLUniformStruct::setMemberData(const std::string& memberName, bool data)
	{
		UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

		if (uniformDescriptor.getType() != ShaderDataType::BOOL)
		{
			Log::cometError("member '" + memberName + "' in uniform struct '" + m_descriptor.getName() + "' is not of type 'BOOL' - value cannot be set");
			CMT_COMET_ASSERT(false);
			return;
		}

		glUniform1i(m_locations[getMemberFullName(uniformDescriptor.getName())], data);
	}

	UniformDescriptor OpenGLUniformStruct::getMemberDescriptor(const std::string& memberName)
	{
		std::vector<UniformDescriptor>::const_iterator it = std::find_if(m_descriptor.begin(), m_descriptor.end(), [=](const UniformDescriptor& uniformDescriptor)
		{
			return uniformDescriptor.getName() == memberName;
		});

		if (it != m_descriptor.end())
			return *it;
		else
		{
			Log::cometError("No member '" + memberName + "' exists in the uniform struct '" + m_descriptor.getName() + "'");
			CMT_COMET_ASSERT(false);
			return UniformDescriptor("INVALID", ShaderDataType::BOOL, 0);
		}
	}

	std::string OpenGLUniformStruct::getMemberFullName(const std::string& memberName)
	{
		std::stringstream ss;
		ss << m_descriptor.getName() << "." << memberName;
		return ss.str();
	}

}