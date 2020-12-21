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

		static Reference<Shader> create(const std::string& filepath);

		virtual void reload() = 0;

		virtual void bind() = 0;

		virtual void setUniformBuffer(uint32_t bindingPoint, void* data) = 0;
		virtual void setUniformBuffer(uint32_t bindingPoint, void* data, uint32_t size, uint32_t offset = 0) = 0;

		virtual void setUniformData(const std::string& uniformName, float data) = 0;
		virtual void setUniformData(const std::string& uniformName, glm::vec2 data) = 0;
		virtual void setUniformData(const std::string& uniformName, glm::vec3 data) = 0;
		virtual void setUniformData(const std::string& uniformName, glm::vec4 data) = 0;

		virtual void setUniformData(const std::string& uniformName, glm::mat3 data) = 0;
		virtual void setUniformData(const std::string& uniformName, glm::mat4 data) = 0;

		virtual void setUniformData(const std::string& uniformName, int32_t data) = 0;
		virtual void setUniformData(const std::string& uniformName, glm::ivec2 data) = 0;
		virtual void setUniformData(const std::string& uniformName, glm::ivec3 data) = 0;
		virtual void setUniformData(const std::string& uniformName, glm::ivec4 data) = 0;

		virtual void setUniformData(const std::string& uniformName, bool data) = 0;

		virtual const std::string& getName() const = 0;
		virtual const std::string& getFilepath() const = 0;
		virtual RendererID getRendererID() const = 0;

	protected:
		static bool optimisation;
	};

}