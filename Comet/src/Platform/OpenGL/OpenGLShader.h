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

		virtual void reload() override;

		void bind() override;

		virtual void setUniformBuffer(uint32_t bindingPoint, void* data) override;
		virtual void setUniformBuffer(uint32_t bindingPoint, void* data, uint32_t size, uint32_t offset = 0) override;

		//TODO: HAVE METHODS TO SET UNIFORM STRUCT MEMBERS
		
		const std::string& getName() const override { return m_name; }
		const std::string& getFilepath() const override { return m_filepath; }
		RendererID getRendererID() const override { return m_rendererID; }

	private:
		std::string getSourceFromFile();
		void load();
		std::unordered_map<GLenum, std::string> getSeparateShaderSources(const std::string& source);
		void setUpShaderUniformsAndResources();

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