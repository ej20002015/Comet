#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/shader.h"

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
		
		const std::string& getName() const override { return m_name; }
		const std::string& getFilepath() const override { return m_filepath; }
		RendererID getRendererID() const override { return m_rendererID; }

	private:
		std::string getSourceFromFile();
		void load(const std::string& source);
		std::unordered_map<GLenum, std::string> getSeparateShaderSources(const std::string& source);
		void getOrCompileVulkanBinaries(std::array<std::vector<uint32_t>, 2>& vulkanBinaries);
		//void getOrCompileOpenGLBinaries(std::array<std::vector<uint32_t>, 2>& openGLBinaries, const std::array<std::vector<uint32_t>, 2>& vulkanBinaries);
		void reflect(const std::array<std::vector<uint32_t>, 2>& vulkanBinaries);

	private:
		RendererID m_rendererID = 0;
		std::string m_name;
		std::string m_filepath;
		std::unordered_map<GLenum, std::string> m_shaderSources;
		bool m_isCompute = false;
	};

}