#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/UniformBuffer.h"
#include "Comet/Core/Buffer.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Comet
{

class OpenGLUniformBuffer : public UniformBuffer
{
public:
	static Reference<OpenGLUniformBuffer> create(const UniformBufferDescriptor& uniformBufferDescriptor);
	OpenGLUniformBuffer(const UniformBufferDescriptor& descriptor);
	OpenGLUniformBuffer() = default;
	~OpenGLUniformBuffer() override;

	void setData(const void* const data, const uint32_t size, const uint32_t offset = 0) override;

	RendererID getRendererID() const override { return m_rendererID; }
	const UniformBufferDescriptor& getDescriptor() const override { return m_descriptor; }

private:
	RendererID m_rendererID = 0;
	UniformBufferDescriptor m_descriptor;
	Unique<Buffer> m_localData;
};

class OpenGLUniformResource : public UniformResource
{
public:
	OpenGLUniformResource(const UniformResourceDescriptor& descriptor, RendererID programID);
	OpenGLUniformResource() = default;

	const UniformResourceDescriptor& getDescriptor() const override { return m_descriptor; }

private:
	UniformResourceDescriptor m_descriptor;
};

class OpenGLUniformStruct : public UniformStruct
{
public:
	OpenGLUniformStruct(const UniformStructDescriptor& descriptor, const RendererID programID);
	OpenGLUniformStruct() = default;

	void setMemberData(const std::string_view memberName, const float data) override;
	void setMemberData(const std::string_view memberName, const glm::vec2& data) override;
	void setMemberData(const std::string_view memberName, const glm::vec3& data) override;
	void setMemberData(const std::string_view memberName, const glm::vec4& data) override;

	void setMemberData(const std::string_view memberName, const glm::mat3& data) override;
	void setMemberData(const std::string_view memberName, const glm::mat4& data) override;

	void setMemberData(const std::string_view memberName, const int32_t data) override;
	void setMemberData(const std::string_view memberName, const glm::ivec2& data) override;
	void setMemberData(const std::string_view memberName, const glm::ivec3& data) override;
	void setMemberData(const std::string_view memberName, const glm::ivec4& data) override;

	void setMemberData(const std::string_view memberName, const bool data) override;

	const UniformStructDescriptor& getDescriptor() const override { return m_descriptor; }

private:
	UniformDescriptor getMemberDescriptor(const std::string_view memberName);
	std::string getMemberFullName(const std::string_view memberName);

private:
	UniformStructDescriptor m_descriptor;
	std::unordered_map<std::string, GLint> m_locations;
};

}