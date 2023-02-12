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
	OpenGLShader(const std::filesystem::path& filepath);
	~OpenGLShader() override;

	void reload() override;

	void bind() override;

	void setUniformData(const std::string_view uniformName, const float data) override       { setUniformDataTemplate(uniformName, data); }
	void setUniformData(const std::string_view uniformName, const glm::vec2& data) override  { setUniformDataTemplate(uniformName, data); }
	void setUniformData(const std::string_view uniformName, const glm::vec3& data) override  { setUniformDataTemplate(uniformName, data); }
	void setUniformData(const std::string_view uniformName, const glm::vec4& data) override  { setUniformDataTemplate(uniformName, data); }

	void setUniformData(const std::string_view uniformName, const glm::mat3& data) override  { setUniformDataTemplate(uniformName, data); }
	void setUniformData(const std::string_view uniformName, const glm::mat4& data) override  { setUniformDataTemplate(uniformName, data); }

	void setUniformData(const std::string_view uniformName, const int32_t data) override     { setUniformDataTemplate(uniformName, data); }
	void setUniformData(const std::string_view uniformName, const glm::ivec2& data) override { setUniformDataTemplate(uniformName, data); }
	void setUniformData(const std::string_view uniformName, const glm::ivec3& data) override { setUniformDataTemplate(uniformName, data); }
	void setUniformData(const std::string_view uniformName, const glm::ivec4& data) override { setUniformDataTemplate(uniformName, data); }

	void setUniformData(const std::string_view uniformName, const bool data) override        { setUniformDataTemplate(uniformName, data); }
		
	const std::string& getName() const override { return m_name; }
	const std::filesystem::path& getFilepath() const override { return m_filepath; }
	RendererID getRendererID() const override { return m_rendererID; }

private:
	std::string getSourceFromFile();
	void load();
	std::unordered_map<ShaderType, std::string> getSeparateShaderSources(const std::string_view source);
	void setUpShaderUniformsAndResources();
		
	//TODO: Support 'nested' structs
	template<typename T>
	void setUniformDataTemplate(const std::string_view uniformName, const T& data)
	{
		const size_t dotOperatorIndex = uniformName.find_first_of(".");
		const std::string_view uniformStructName = uniformName.substr(0, dotOperatorIndex);
		const std::string_view uniformStructMemberName = uniformName.substr(dotOperatorIndex + 1);

		CMT_COMET_ASSERT_MESSAGE(m_uniformStructs.find(static_cast<std::string>(uniformStructName)) != m_uniformStructs.end(),
			fmt::format("Cannot find uniform struct '{0}' in shader '{1}' - value cannot be set", uniformStructName, m_name));

		m_uniformStructs.at(static_cast<std::string>(uniformStructName)).setMemberData(uniformStructMemberName, data);
	}

private:
	static constexpr std::string_view SHADER_TYPE_DECLARATION_TOKEN = "#type";

	RendererID m_rendererID = 0;
	std::string m_name;
	std::filesystem::path m_filepath;
	std::unordered_map<ShaderType, std::string> m_shaderSources;
	bool m_isCompute = false;

	std::vector<SpirvShaderInformation> m_shaderInformationVulkan;

	std::unordered_map<std::string, OpenGLUniformStruct> m_uniformStructs;
	std::unordered_map<std::string, OpenGLUniformResource> m_resources;
};

}