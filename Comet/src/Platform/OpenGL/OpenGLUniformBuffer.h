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

		void setData(void* data, uint32_t size, uint32_t offset = 0) override;

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

		void setMemberData(const std::string& memberName, float data) override;
		void setMemberData(const std::string& memberName, glm::vec2 data) override;
		void setMemberData(const std::string& memberName, glm::vec3 data) override;
		void setMemberData(const std::string& memberName, glm::vec4 data) override;

		void setMemberData(const std::string& memberName, glm::mat3 data) override;
		void setMemberData(const std::string& memberName, glm::mat4 data) override;

		void setMemberData(const std::string& memberName, int32_t data) override;
		void setMemberData(const std::string& memberName, glm::ivec2 data) override;
		void setMemberData(const std::string& memberName, glm::ivec3 data) override;
		void setMemberData(const std::string& memberName, glm::ivec4 data) override;

		void setMemberData(const std::string& memberName, bool data) override;

		const UniformStructDescriptor& getDescriptor() const override { return m_descriptor; }

	private:
		UniformDescriptor getMemberDescriptor(const std::string& memberName);
		std::string getMemberFullName(const std::string& memberName);

	private:
		UniformStructDescriptor m_descriptor;
		std::unordered_map<std::string, GLint> m_locations;
	};

}