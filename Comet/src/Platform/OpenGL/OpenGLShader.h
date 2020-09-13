#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/shader.h"
#include "Platform/SPIR-V/SpirvTools.h"

#include "OpenGLUniformBuffer.h"

#include "glad/glad.h"

namespace Comet
{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		~OpenGLShader() override;

		void reload() override;

		void bind() override;

		void setUniformBuffer(uint32_t bindingPoint, void* data) override;
		void setUniformBuffer(uint32_t bindingPoint, void* data, uint32_t size, uint32_t offset = 0) override;

		void setUniformData(const std::string& uniformName, float data) override      { setUniformDataTemplate(uniformName, data); }
		void setUniformData(const std::string& uniformName, glm::vec2 data) override  { setUniformDataTemplate(uniformName, data); }
		void setUniformData(const std::string& uniformName, glm::vec3 data) override  { setUniformDataTemplate(uniformName, data); }
		void setUniformData(const std::string& uniformName, glm::vec4 data) override  { setUniformDataTemplate(uniformName, data); }

		void setUniformData(const std::string& uniformName, glm::mat3 data) override  { setUniformDataTemplate(uniformName, data); }
		void setUniformData(const std::string& uniformName, glm::mat4 data) override  { setUniformDataTemplate(uniformName, data); }

		void setUniformData(const std::string& uniformName, int32_t data) override    { setUniformDataTemplate(uniformName, data); }
		void setUniformData(const std::string& uniformName, glm::ivec2 data) override { setUniformDataTemplate(uniformName, data); }
		void setUniformData(const std::string& uniformName, glm::ivec3 data) override { setUniformDataTemplate(uniformName, data); }
		void setUniformData(const std::string& uniformName, glm::ivec4 data) override { setUniformDataTemplate(uniformName, data); }

		void setUniformData(const std::string& uniformName, bool data) override       { setUniformDataTemplate(uniformName, data); }
		
		const std::string& getName() const override { return m_name; }
		const std::string& getFilepath() const override { return m_filepath; }
		RendererID getRendererID() const override { return m_rendererID; }

	private:
		std::string getSourceFromFile();
		void load();
		std::unordered_map<GLenum, std::string> getSeparateShaderSources(const std::string& source);
		void setUpShaderUniformsAndResources();
		
		template<typename T>
		void setUniformDataTemplate(const std::string& uniformName, T data)
		{
			size_t dotOperatorIndex = uniformName.find_first_of(".");
			std::string uniformStructName = uniformName.substr(0, dotOperatorIndex);
			std::string uniformStructMemberName = uniformName.substr(dotOperatorIndex + 1);
			if (m_uniformStructs.find(uniformStructName) != m_uniformStructs.end())
				m_uniformStructs.at(uniformStructName).setMemberData(uniformStructMemberName, data);
			else
			{
				Log::cometError("Cannot find uniform struct '{0}' in shader '{1}' - value cannot be set", uniformStructName, m_name);
				CMT_COMET_ASSERT(false);
			}
		}

	private:
		RendererID m_rendererID = 0;
		std::string m_name;
		std::string m_filepath;
		std::unordered_map<GLenum, std::string> m_shaderSources;
		bool m_isCompute = false;

		std::vector<SpirvShaderInformation> m_shaderInformationVulkan;

		//TODO: MAYBE NOT A MAP?
		static std::unordered_map<uint32_t, Reference<OpenGLUniformBuffer>> s_uniformBuffers;
		std::unordered_map<std::string, OpenGLUniformStruct> m_uniformStructs;
		std::unordered_map<std::string, OpenGLUniformResource> m_resources;
	};

}