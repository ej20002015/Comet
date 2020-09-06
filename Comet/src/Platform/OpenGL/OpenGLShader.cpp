#include "CometPCH.h"
#include "OpenGLShader.h"

#include "Platform/SPIR-V/SpirvTools.h"

namespace Comet
{

	std::unordered_map<uint32_t, UniformBuffer> OpenGLShader::s_uniformBuffers;

	static GLenum getGLShaderType(const std::string& typeToken)
	{
		if (typeToken == "vertex")
			return GL_VERTEX_SHADER;
		else if (typeToken == "fragment")
			return GL_FRAGMENT_SHADER;
		else if (typeToken == "pixel")
			return GL_FRAGMENT_SHADER;
		else if (typeToken == "compute")
			return GL_COMPUTE_SHADER;

		CMT_COMET_ASSERT_MESSAGE(false, "Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_filepath(filepath)
	{
		//Set name of shader from the filepath (all file suffixes are stripped)
		std::filesystem::path path = filepath;
		std::filesystem::path name = path.stem();

		while (name.string().find(".") != std::string::npos)
			name = name.stem();

		m_name = name.string();

		reload();
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_rendererID);
	}

	void OpenGLShader::reload()
	{
		std::string source = getSourceFromFile();
		//Split source by the type of shader
		m_shaderSources = getSeparateShaderSources(source);

		load();
	}

	void OpenGLShader::bind()
	{
		glUseProgram(m_rendererID);
	}

	std::string OpenGLShader::getSourceFromFile()
	{
		std::string source;
		std::ifstream input(m_filepath, std::ios::in | std::ios::binary);
		CMT_COMET_ASSERT_MESSAGE(input, "Cannot open shader file");
		
		//Get size of file
		input.seekg(0, input.end);
		uint64_t charCount = static_cast<uint64_t>(input.tellg());
		input.seekg(0, input.beg);
		
		source.resize(charCount);
		input.read(source.data(), charCount);

		input.close();
		return source;
	}

	void OpenGLShader::load()
	{	
		std::array<std::vector<uint32_t>, 2> binaries;
		SpirvShaderInformation vertexShaderInfo = SpirvTools::compileAndReflect(m_shaderSources.at(GL_VERTEX_SHADER), m_filepath, m_name, ShaderType::VERTEX, ShaderEnvironment::OPENGL, optimisation);
		SpirvShaderInformation fragmentShaderInfo = SpirvTools::compileAndReflect(m_shaderSources.at(GL_FRAGMENT_SHADER), m_filepath, m_name, ShaderType::FRAGMENT, ShaderEnvironment::OPENGL, optimisation);

		binaries[0] = vertexShaderInfo.binary;
		binaries[1] = fragmentShaderInfo.binary;

		//TODO: NEEDS TO BE MOVED INTO OWN OpenGLUniformBufferClass

		for (UniformBuffer uniformBuffer : vertexShaderInfo.uniformBuffers)
		{
			if (s_uniformBuffers.find(uniformBuffer.getBindingPoint()) == s_uniformBuffers.end())
			{
				uint32_t bufferID;
				glCreateBuffers(1, &bufferID);
				glNamedBufferData(bufferID, uniformBuffer.getSize(), nullptr, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_UNIFORM_BUFFER, uniformBuffer.getBindingPoint(), bufferID);
				uniformBuffer.setRendererID(bufferID);
				s_uniformBuffers[uniformBuffer.getBindingPoint()] = uniformBuffer;
			}
		}

		glm::vec4 color(0.1f, 0.8f, 0.8f, 1.0f);

		for (UniformBuffer uniformBuffer : fragmentShaderInfo.uniformBuffers)
		{
			if (s_uniformBuffers.find(uniformBuffer.getBindingPoint()) == s_uniformBuffers.end())
			{
				uint32_t bufferID;
				glCreateBuffers(1, &bufferID);
				glNamedBufferData(bufferID, uniformBuffer.getSize(), nullptr, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_UNIFORM_BUFFER, uniformBuffer.getBindingPoint(), bufferID);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
				uniformBuffer.setRendererID(bufferID);
				s_uniformBuffers[uniformBuffer.getBindingPoint()] = uniformBuffer;
			}

			if (uniformBuffer.getBindingPoint() == 0)
			{
				glNamedBufferSubData(uniformBuffer.getRendererID(), 0, uniformBuffer.getSize(), (const void*)&color[0]);
			}
		}

		//TODO: update

		if (m_rendererID)
			glDeleteProgram(m_rendererID);

		m_rendererID = glCreateProgram();

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderBinary(1, &vertexShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, binaries[0].data(), static_cast<GLsizei>(binaries[0].size() * sizeof(uint32_t)));
		glSpecializeShader(vertexShaderID, "main", 0, nullptr, nullptr);
		glAttachShader(m_rendererID, vertexShaderID);

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderBinary(1, &fragmentShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, binaries[1].data(), static_cast<GLsizei>(binaries[1].size() * sizeof(uint32_t)));
		glSpecializeShader(fragmentShaderID, "main", 0, nullptr, nullptr);
		glAttachShader(m_rendererID, fragmentShaderID);

		glLinkProgram(m_rendererID);

		GLint isLinked;
		glGetProgramiv(m_rendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, &infoLog[0]);
			Log::cometError("Shader compilation failed ({0}):\n{1}", m_filepath, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_rendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
		}

		glDetachShader(m_rendererID, vertexShaderID);
		glDetachShader(m_rendererID, fragmentShaderID);
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::getSeparateShaderSources(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		std::string shaderTypeDeclarationToken = "#type";
		size_t shaderTypeDeclarationTokenLength = shaderTypeDeclarationToken.size();
		size_t position = source.find(shaderTypeDeclarationToken, 0);
		while (position != std::string::npos)
		{
			//Identify type of shader
			size_t endOfLine = source.find_first_of("\n\r", position);
			CMT_COMET_ASSERT_MESSAGE(endOfLine != std::string::npos, "Syntax error - No source code under shader type declaration");
			size_t startOfTypeToken = position + shaderTypeDeclarationTokenLength + 1;
			std::string typeToken = source.substr(startOfTypeToken, endOfLine - startOfTypeToken);
			CMT_COMET_ASSERT_MESSAGE(typeToken == "vertex" || typeToken == "fragment" || typeToken == "pixel" || typeToken == "compute", "Unknown shader type");

			//Create a shader source entry
			size_t startOfSourceCode = source.find_first_not_of("\n\r", endOfLine);
			CMT_COMET_ASSERT_MESSAGE(startOfSourceCode != std::string::npos, "Syntax error - No source code under shader type declaration");
			position = source.find(shaderTypeDeclarationToken, startOfSourceCode);
			std::string sourceCode = source.substr(startOfSourceCode, ((position != std::string::npos) ? position - 1 : source.size()) - startOfSourceCode);
			GLenum GLType = getGLShaderType(typeToken);
			shaderSources[GLType] = sourceCode;

			//Compute shader can only be on its own
			if (GLType == GL_COMPUTE_SHADER)
			{
				m_isCompute = true;
				CMT_COMET_ASSERT_MESSAGE(shaderSources.size() < 2, "Compute shaders must be on their own");
				break;
			}
		}

		CMT_COMET_ASSERT_MESSAGE(shaderSources.size(), "No shader source code found - make sure to label the types of shaders using '#type [shaderType]'")
		return shaderSources;
	}

}
