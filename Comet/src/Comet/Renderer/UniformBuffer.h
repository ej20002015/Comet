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

	virtual void setData(const void* const data, const uint32_t size, const uint32_t offset = 0) = 0;
	void setData(const void* const data) { setData(data, getDescriptor().getSize()); }

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

	virtual void setMemberData(const std::string_view memberName, const float data) = 0;
	virtual void setMemberData(const std::string_view memberName, const glm::vec2& data) = 0;
	virtual void setMemberData(const std::string_view memberName, const glm::vec3& data) = 0;
	virtual void setMemberData(const std::string_view memberName, const glm::vec4& data) = 0;

	virtual void setMemberData(const std::string_view memberName, const glm::mat3& data) = 0;
	virtual void setMemberData(const std::string_view memberName, const glm::mat4& data) = 0;

	virtual void setMemberData(const std::string_view memberName, const int32_t data) = 0;
	virtual void setMemberData(const std::string_view memberName, const glm::ivec2& data) = 0;
	virtual void setMemberData(const std::string_view memberName, const glm::ivec3& data) = 0;
	virtual void setMemberData(const std::string_view memberName, const glm::ivec4& data) = 0;

	virtual void setMemberData(const std::string_view memberName, const bool data) = 0;

	virtual const UniformStructDescriptor& getDescriptor() const = 0;
};

}