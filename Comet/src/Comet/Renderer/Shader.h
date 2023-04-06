#pragma once
#include "CometPCH.h"

#include "RendererAPI.h"
#include "UniformBuffer.h"

namespace Comet
{

class Shader
{
public:
	virtual ~Shader() = default;

	static Reference<Shader> create(const std::filesystem::path& filepath);

	virtual void reload() = 0;

	virtual void bind() = 0;

	static void setUniformBuffer(const uint32_t bindingPoint, const void* const data);
	static void setUniformBuffer(const uint32_t bindingPoint, const void* const data, const uint32_t size, const uint32_t offset = 0);

	virtual void setUniformData(const std::string_view uniformName, const float data) = 0;
	virtual void setUniformData(const std::string_view uniformName, const glm::vec2& data) = 0;
	virtual void setUniformData(const std::string_view uniformName, const glm::vec3& data) = 0;
	virtual void setUniformData(const std::string_view uniformName, const glm::vec4& data) = 0;

	virtual void setUniformData(const std::string_view uniformName, const glm::mat3& data) = 0;
	virtual void setUniformData(const std::string_view uniformName, const glm::mat4& data) = 0;

	virtual void setUniformData(const std::string_view uniformName, const int32_t data) = 0;
	virtual void setUniformData(const std::string_view uniformName, const glm::ivec2& data) = 0;
	virtual void setUniformData(const std::string_view uniformName, const glm::ivec3& data) = 0;
	virtual void setUniformData(const std::string_view uniformName, const glm::ivec4& data) = 0;

	virtual void setUniformData(const std::string_view uniformName, const bool data) = 0;

	virtual const std::string& getName() const = 0;
	virtual const std::filesystem::path& getFilepath() const = 0;
	virtual RendererID getRendererID() const = 0;

protected:
	constexpr static bool OPTIMISATION = true;
	static std::unordered_map<uint32_t, Reference<UniformBuffer>> s_uniformBuffers;
};

}