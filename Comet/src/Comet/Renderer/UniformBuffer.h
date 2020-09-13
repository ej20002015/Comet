#pragma once
#include "CometPCH.h"

#include "UniformBufferDescriptor.h"
#include "RendererAPI.h"

namespace Comet
{

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void setData(void* data, uint32_t size, uint32_t offset = 0) = 0;

		virtual RendererID getRendererID() const = 0;
		virtual const UniformBufferDescriptor& getDescriptor() const = 0;
	};

	class UniformResource
	{
	public:
		virtual ~UniformResource() = default;

		virtual const UniformResourceDescriptor& getDescriptor() const = 0;
	};

	class UniformStruct
	{
	public:
		virtual ~UniformStruct() = default;

		virtual void setMemberData(const std::string& memberName, float data) = 0;
		virtual void setMemberData(const std::string& memberName, glm::vec2 data) = 0;
		virtual void setMemberData(const std::string& memberName, glm::vec3 data) = 0;
		virtual void setMemberData(const std::string& memberName, glm::vec4 data) = 0;

		virtual void setMemberData(const std::string& memberName, glm::mat3 data) = 0;
		virtual void setMemberData(const std::string& memberName, glm::mat4 data) = 0;

		virtual void setMemberData(const std::string& memberName, int32_t data) = 0;
		virtual void setMemberData(const std::string& memberName, glm::ivec2 data) = 0;
		virtual void setMemberData(const std::string& memberName, glm::ivec3 data) = 0;
		virtual void setMemberData(const std::string& memberName, glm::ivec4 data) = 0;

		virtual void setMemberData(const std::string& memberName, bool data) = 0;

		virtual const UniformStructDescriptor& getDescriptor() const = 0;
	};

}