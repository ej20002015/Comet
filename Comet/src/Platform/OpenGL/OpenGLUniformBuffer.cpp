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

void OpenGLUniformBuffer::setData(const void* const data, const uint32_t size, const uint32_t offset)
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
		std::string memberFullName = getMemberFullName(member.getName());
		GLint location = glGetUniformLocation(programID, memberFullName.c_str());
		CMT_COMET_ASSERT_MESSAGE(location != -1, fmt::format("Cannot get location of uniform {0}", m_descriptor.getName()))
		m_locations[memberFullName] = location;
	}
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const float data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);
	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::FLOAT, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::FLOAT))

	glUniform1f(m_locations[getMemberFullName(uniformDescriptor.getName())], data);
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const glm::vec2& data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);

	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::FLOAT2, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::FLOAT2))
	glUniform2f(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y);
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const glm::vec3& data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);
	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::FLOAT3, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::FLOAT3))

	glUniform3f(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y, data.z);
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const glm::vec4& data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);
	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::FLOAT4, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::FLOAT4))

	glUniform4f(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y, data.z, data.w);
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const glm::mat3& data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);
	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::MAT3, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::MAT3))

	glUniform3fv(m_locations[getMemberFullName(uniformDescriptor.getName())], 1, &data[0][0]);
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const glm::mat4& data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);
	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::MAT4, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::MAT4))

	glUniform4fv(m_locations[getMemberFullName(uniformDescriptor.getName())], 1, &data[0][0]);
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const int32_t data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);
	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::INT, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::INT))

	glUniform1i(m_locations[getMemberFullName(uniformDescriptor.getName())], data);
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const glm::ivec2& data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);
	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::INT2, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::INT2))

	glUniform2i(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y);
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const glm::ivec3& data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);
	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::INT3, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::INT3))

	glUniform3i(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y, data.z);
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const glm::ivec4& data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);
	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::INT4, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::INT4))

	glUniform4i(m_locations[getMemberFullName(uniformDescriptor.getName())], data.x, data.y, data.z, data.w);
}

void OpenGLUniformStruct::setMemberData(const std::string_view memberName, const bool data)
{
	const UniformDescriptor uniformDescriptor = getMemberDescriptor(memberName);
	CMT_COMET_ASSERT_MESSAGE(uniformDescriptor.getType() == ShaderDataType::BOOL, fmt::format(TYPE_ERROR_STR, memberName, m_descriptor.getName(), ShaderDataType::BOOL))

	glUniform1i(m_locations[getMemberFullName(uniformDescriptor.getName())], data);
}

UniformDescriptor OpenGLUniformStruct::getMemberDescriptor(const std::string_view memberName)
{
	std::vector<UniformDescriptor>::const_iterator it = std::find_if(m_descriptor.begin(), m_descriptor.end(), [memberName](const UniformDescriptor& uniformDescriptor)
	{
		return uniformDescriptor.getName() == memberName;
	});

	CMT_COMET_ASSERT_MESSAGE(it != m_descriptor.end(), fmt::format("No member '{0}' exists in the uniform struct '{1}'", memberName, m_descriptor.getName()));

	return *it;
}

std::string OpenGLUniformStruct::getMemberFullName(const std::string_view memberName)
{
	std::stringstream ss;
	ss << m_descriptor.getName() << "." << memberName;
	return ss.str();
}

OpenGLUniformResource::OpenGLUniformResource(const UniformResourceDescriptor& descriptor, RendererID programID)
	: m_descriptor(descriptor)
{
	const GLint location = glGetUniformLocation(programID, m_descriptor.getName().data());
	CMT_COMET_ASSERT_MESSAGE(location != -1, fmt::format("Cannot get location of uniform {0}", m_descriptor.getName()));

	glUseProgram(programID);
	glUniform1i(location, m_descriptor.getBindingPoint());
}

}