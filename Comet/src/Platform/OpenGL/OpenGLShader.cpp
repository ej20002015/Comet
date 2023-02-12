#include "CometPCH.h"
#include "OpenGLShader.h"

#include "Comet/Renderer/ShaderType.h"

namespace Comet
{

static GLenum getGLShaderType(const ShaderType type)
{
	switch (type)
	{
		case ShaderType::VERTEX:   return GL_VERTEX_SHADER; break;
		case ShaderType::FRAGMENT: return GL_FRAGMENT_SHADER; break;
		case ShaderType::COMPUTE:  return GL_COMPUTE_SHADER; break;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown shader type");
		return 0;
		break;
	}
}

OpenGLShader::OpenGLShader(const std::filesystem::path& filepath)
	: m_filepath(filepath)
{
	std::filesystem::path name = m_filepath.stem();
	for (; name.string().find(".") != std::string::npos; name = name.stem())

	m_name = name.string();

	Log::cometInfo("Creating shader {0}", m_name);

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
	if (!input)
		throw CometException() << fmt::format("Cannot open shader file '{0}'",  m_filepath.string());
		
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
	//TODO: Make dynamic - only works with Vertex/Fragment shader pairing at the moment

	std::array<std::vector<uint32_t>, 2> binaries;

	//All shaders are written in vulkan so compile vulkan and get reflection info
	const SpirvShaderInformation vertexShaderInfoVulkan = SpirvTools::compileAndReflect(m_shaderSources.at(ShaderType::VERTEX), m_filepath, m_name, ShaderType::VERTEX, ShaderEnvironment::VULKAN, OPTIMISATION);
	m_shaderInformationVulkan.push_back(vertexShaderInfoVulkan);
	const SpirvShaderInformation fragmentShaderInfoVulkan = SpirvTools::compileAndReflect(m_shaderSources.at(ShaderType::FRAGMENT), m_filepath, m_name, ShaderType::FRAGMENT, ShaderEnvironment::VULKAN, OPTIMISATION);
	m_shaderInformationVulkan.push_back(fragmentShaderInfoVulkan);

	//Get OpenGL source version of the shader code
	const std::string vertexShaderSourceOpenGL = SpirvTools::getOpenGLFromBinary(vertexShaderInfoVulkan.binary);
	const std::string fragmentShaderSourceOpenGL = SpirvTools::getOpenGLFromBinary(fragmentShaderInfoVulkan.binary);

	//Get spirv OpenGL code
	binaries[0] = SpirvTools::compile(vertexShaderSourceOpenGL, m_filepath, m_name, ShaderType::VERTEX, ShaderEnvironment::OPENGL, OPTIMISATION);
	binaries[1] = SpirvTools::compile(fragmentShaderSourceOpenGL, m_filepath, m_name, ShaderType::FRAGMENT, ShaderEnvironment::OPENGL, OPTIMISATION);

	if (m_rendererID)
		glDeleteProgram(m_rendererID);

	m_rendererID = glCreateProgram();

	const GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderBinary(1, &vertexShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, binaries[0].data(), static_cast<GLsizei>(binaries[0].size() * sizeof(uint32_t)));
	glSpecializeShader(vertexShaderID, "main", 0, nullptr, nullptr);
	glAttachShader(m_rendererID, vertexShaderID);

	const GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
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
		Log::cometError("[SHADER LINK ERROR] Shader linking failed ({0}):\n{1}", m_filepath.string(), &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(m_rendererID);
		// Don't leak shaders either.
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	glDetachShader(m_rendererID, vertexShaderID);
	glDeleteShader(vertexShaderID);
	glDetachShader(m_rendererID, fragmentShaderID);
	glDeleteShader(fragmentShaderID);

	//TODO: DO ALL WRITING OF CACHED SHADERS HERE, WHERE WE KNOW THEY ARE CORRECT (NO LINKING OR COMPILING ERRORS)

	setUpShaderUniformsAndResources();
}

std::unordered_map<ShaderType, std::string> OpenGLShader::getSeparateShaderSources(const std::string_view source)
{
	std::unordered_map<ShaderType, std::string> shaderSources;

	size_t position = source.find(SHADER_TYPE_DECLARATION_TOKEN, 0);
	if (position == std::string::npos)
		throw CometException() << "[SHADER SYNTAX ERROR] No shader type declaration found - make sure to label the types of shaders using '#type [shaderType]'";

	while (position != std::string::npos)
	{
		//Identify type of shader
		size_t endOfLine = source.find_first_of("\n\r", position);
		if (endOfLine == std::string::npos)
			throw CometException() << "[SHADER SYNTAX ERROR] No source code under shader type declaration";

		const size_t startOfTypeToken = position + SHADER_TYPE_DECLARATION_TOKEN.size() + 1;
		const std::string_view typeToken = source.substr(startOfTypeToken, endOfLine - startOfTypeToken);
		if (std::find(SHADER_TYPE_STRS.begin(), SHADER_TYPE_STRS.end(), typeToken) == SHADER_TYPE_STRS.end())
			throw CometException() << "[SHADER SYNTAX ERROR] shaderType must be one of" << containerToStr(SHADER_TYPE_STRS);

		const ShaderType shaderType = strToShaderType(typeToken);

		//Create a shader source entry
		const size_t startOfSourceCode = source.find_first_not_of("\n\r", endOfLine);
		if (startOfSourceCode == std::string::npos)
			throw CometException() << "[SHADER SYNTAX ERROR] No source code under shader type declaration";

		position = source.find(SHADER_TYPE_DECLARATION_TOKEN, startOfSourceCode);
		const std::string_view sourceCode = source.substr(startOfSourceCode, ((position != std::string::npos) ? position - 1 : source.size()) - startOfSourceCode);
		
		shaderSources.insert({ shaderType, std::string(sourceCode) });

		//Compute shader can only be on its own
		if (shaderType == ShaderType::COMPUTE)
		{
			if (shaderSources.size() > 1)
				throw CometException() << "[SHADER SYNTAX ERROR] Compute shaders must be on their own";

			m_isCompute = true;
			break;
		}
	}

	return shaderSources;
}

void OpenGLShader::setUpShaderUniformsAndResources()
{
	//Loop through all reflected shaders

	for (const SpirvShaderInformation& shaderInfoVulkan : m_shaderInformationVulkan)
	{

		//Set up uniform buffers

		for (const UniformBufferDescriptor& uniformBufferDescriptor : shaderInfoVulkan.uniformBuffers)
		{
			if (s_uniformBuffers.find(uniformBufferDescriptor.getBindingPoint()) == s_uniformBuffers.end())
			{
				Reference<UniformBuffer> uniformBuffer = OpenGLUniformBuffer::create(uniformBufferDescriptor);
				s_uniformBuffers.insert({ uniformBuffer->getDescriptor().getBindingPoint(), uniformBuffer });
			}
		}

		//Set up uniform structs

		for (const UniformStructDescriptor& uniformStructDescriptor : shaderInfoVulkan.uniformStructs)
		{
			OpenGLUniformStruct uniformStruct(uniformStructDescriptor, m_rendererID);
			m_uniformStructs.insert({ std::string(uniformStructDescriptor.getName()), uniformStruct });
		}

		//Set up shader resources

		for (const UniformResourceDescriptor& uniformResourceDescriptor : shaderInfoVulkan.uniformResources)
		{
			OpenGLUniformResource uniformResource(uniformResourceDescriptor, m_rendererID);
			m_resources.insert({ std::string(uniformResourceDescriptor.getName()), uniformResource });
		}
	}
}

}
