#include "CometPCH.h"
#include "OpenGLShader.h"

//spir-v/shaderc/SPIRV-Cross
#include "shaderc/shaderc.hpp"
#include "spvc/spvc.hpp"
#include "spirv_cross/spirv_glsl.hpp"

namespace Comet
{

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
		load(source);
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

	void OpenGLShader::load(const std::string& source)
	{
		//Split source into the types of shaders
		m_shaderSources = getSeparateShaderSources(source);
		
		std::array<std::vector<uint32_t>, 2> vulkanBinaries;
		getOrCompileVulkanBinaries(vulkanBinaries);

		reflect(vulkanBinaries);

		//TODO: update

		if (m_rendererID)
			glDeleteProgram(m_rendererID);

		m_rendererID = glCreateProgram();

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderBinary(1, &vertexShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, vulkanBinaries[0].data(), static_cast<GLsizei>(vulkanBinaries[0].size() * sizeof(uint32_t)));
		glSpecializeShader(vertexShaderID, "main", 0, nullptr, nullptr);
		glAttachShader(m_rendererID, vertexShaderID);

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderBinary(1, &fragmentShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, vulkanBinaries[1].data(), static_cast<GLsizei>(vulkanBinaries[1].size() * sizeof(uint32_t)));
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

		//getOrCompileOpenGLBinaries(openGLBinaries, vulkanBinaries);

		glDetachShader(m_rendererID, vertexShaderID);
		glDetachShader(m_rendererID, fragmentShaderID);

		//Create openGL program and shaders
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

	void OpenGLShader::getOrCompileVulkanBinaries(std::array<std::vector<uint32_t>, 2>& vulkanBinaries)
	{
		//See if vulkan binaries have already been generated for this shader, and if so retrieve them

		//Vertex
		{
			std::filesystem::path path = m_filepath;
			path = path.parent_path() / "cached" / (m_name + ".vulkanBinary.vert");
			std::string cachedBinaryPath = path.string();

			std::ifstream input(cachedBinaryPath, std::ios::binary);
			if (input)
			{
				Log::cometInfo("Retrieving cached shader at {0}", cachedBinaryPath);
				input.seekg(0, input.end);
				uint64_t size = static_cast<uint64_t>(input.tellg());
				input.seekg(0, input.beg);

				vulkanBinaries[0] = std::vector<uint32_t>(size / sizeof(uint32_t));
				input.read((char*)vulkanBinaries[0].data(), size);
				input.close();
			}
		}

		//Fragment
		{
			std::filesystem::path path = m_filepath;
			path = path.parent_path() / "cached" / (m_name + ".vulkanBinary.frag");
			std::string cachedBinaryPath = path.string();

			std::ifstream input(cachedBinaryPath, std::ios::binary);
			if (input)
			{
				Log::cometInfo("Retrieving cached shader at {0}", cachedBinaryPath);
				input.seekg(0, input.end);
				uint64_t size = static_cast<uint64_t>(input.tellg());
				input.seekg(0, input.beg);

				vulkanBinaries[1] = std::vector<uint32_t>(size / sizeof(uint32_t));
				input.read((char*)vulkanBinaries[1].data(), size);
				input.close();
			}
		}

		//If cached vulkan binaries have not be found then create them

		//Vertex
		if (vulkanBinaries[0].size() == 0)
		{
			Log::cometInfo("Compiling and caching shader {0}", m_name);
			//Compile binary
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

			if (optimisation)
				options.SetOptimizationLevel(shaderc_optimization_level_performance);

			const std::string& source = m_shaderSources.at(GL_VERTEX_SHADER);
			std::string name = m_name + " [VERTEX]";
			shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, shaderc_vertex_shader, name.c_str(), options);

			if (result.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				Log::cometError(result.GetErrorMessage());
				CMT_COMET_ASSERT(false);
			}

			vulkanBinaries[0] = std::vector<uint32_t>(result.cbegin(), result.cend());

			//Cache compiled binary
			std::filesystem::path path = m_filepath;
			std::filesystem::path cachedDirectoryPath = path.parent_path() / "cached";
			path = cachedDirectoryPath / (m_name + ".vulkanBinary.vert");
			std::string cachedBinaryPath = path.string();

			//Create 'cached' directory if it does not exist
			std::filesystem::create_directory(cachedDirectoryPath);

			std::ofstream output(cachedBinaryPath, std::ios::binary);
			if (!output)
			{
				Log::cometError("Cannot create file to cache shader binary at {0}", cachedBinaryPath);
				CMT_COMET_ASSERT(false);
			}

			output.write((char*)vulkanBinaries[0].data(), vulkanBinaries[0].size() * sizeof(uint32_t));
			output.close();
		}

		//Fragment
		if (vulkanBinaries[1].size() == 0)
		{
			Log::cometInfo("Compiling and caching shader {0}", m_name);
			//Compile binary
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

			if (optimisation)
				options.SetOptimizationLevel(shaderc_optimization_level_performance);

			const std::string& source = m_shaderSources.at(GL_FRAGMENT_SHADER);
			std::string name = m_name + " [FRAGMENT]";
			shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, shaderc_fragment_shader, name.c_str(), options);

			if (result.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				Log::cometError(result.GetErrorMessage());
				CMT_COMET_ASSERT(false);
			}

			vulkanBinaries[1] = std::vector<uint32_t>(result.cbegin(), result.cend());

			//Cache compiled binary
			std::filesystem::path path = m_filepath;
			std::filesystem::path cachedDirectoryPath = path.parent_path() / "cached";
			path = cachedDirectoryPath / (m_name + ".vulkanBinary.frag");
			std::string cachedBinaryPath = path.string();

			//Create 'cached' directory if it does not exist
			std::filesystem::create_directory(cachedDirectoryPath);

			std::ofstream output(cachedBinaryPath, std::ios::binary);
			if (!output)
			{
				Log::cometError("Cannot create file to cache shader binary at {0}", cachedBinaryPath);
				CMT_COMET_ASSERT(false);
			}

			output.write((char*)vulkanBinaries[1].data(), vulkanBinaries[1].size() * sizeof(uint32_t));
			output.close();
		}
	}

	void OpenGLShader::reflect(const std::array<std::vector<uint32_t>, 2>& vulkanBinaries)
	{
		spirv_cross::CompilerGLSL glsl(vulkanBinaries[0]);

		// The SPIR-V is now parsed, and we can perform reflection on it.
		spirv_cross::ShaderResources resources = glsl.get_shader_resources();

		// Get all sampled images in the shader.
		for (auto& resource : resources.uniform_buffers)
		{
			unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
			unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);

			auto& bufferType = glsl.get_type(resource.base_type_id);
			int memberCount = bufferType.member_types.size();
			uint32_t bindingPoint = glsl.get_decoration(resource.id, spv::DecorationBinding);
			std::string name = resource.name;
			uint32_t bufferSize = glsl.get_declared_struct_size(bufferType);


			Log::cometTrace("Image {0} at set = {1}, binding = {2}", resource.name.c_str(), set, binding);

			//// Modify the decoration to prepare it for GLSL.
			//glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

			//// Some arbitrary remapping if we want.
			//glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
		}

		// Set some options.
		//spirv_cross::CompilerGLSL::Options options;
		//options.version = 310;
		//options.es = true;
		//glsl.set_options(options);

		//// Compile to GLSL, ready to give to GL driver.
		//std::string source = glsl.compile();
	}

	//void OpenGLShader::getOrCompileOpenGLBinaries(std::array<std::vector<uint32_t>, 2>& openGLBinaries, const std::array<std::vector<uint32_t>, 2>& vulkanBinaries)
	//{
	//	//See if OpenGL binaries have already been generated for this shader, and if so retrieve them

	//	//Vertex
	//	{
	//		std::filesystem::path path = m_filepath;
	//		path = path.parent_path() / "cached" / (m_name + ".OpenGLBinary.vert");
	//		std::string cachedBinaryPath = path.string();

	//		std::ifstream input(cachedBinaryPath, std::ios::binary);
	//		if (input)
	//		{
	//			input.seekg(0, input.end);
	//			uint64_t size = static_cast<uint64_t>(input.tellg());
	//			input.seekg(0, input.beg);

	//			openGLBinaries[0] = std::vector<uint32_t>(size / sizeof(uint32_t));
	//			input.read((char*)openGLBinaries[0].data(), size);
	//			input.close();
	//		}
	//	}

	//	//Fragment
	//	{
	//		std::filesystem::path path = m_filepath;
	//		path = path.parent_path() / "cached" / (m_name + ".OpenGLBinary.frag");
	//		std::string cachedBinaryPath = path.string();

	//		std::ifstream input(cachedBinaryPath, std::ios::binary);
	//		if (input)
	//		{
	//			input.seekg(0, input.end);
	//			uint64_t size = static_cast<uint64_t>(input.tellg());
	//			input.seekg(0, input.beg);

	//			openGLBinaries[1] = std::vector<uint32_t>(size / sizeof(uint32_t));
	//			input.read((char*)openGLBinaries[1].data(), size);
	//			input.close();
	//		}
	//	}

	//	//If cached OpenGL binaries have not be found then create them

	//	//Vertex
	//	if (openGLBinaries[0].size() == 0)
	//	{
	//		//Compile binary
	//		shaderc::Compiler compiler;
	//		shaderc::CompileOptions options;
	//		options.SetTargetEnvironment(shaderc_target_env_opengl_compat, shaderc_env_version_opengl_4_5);

	//		if (optimisation)
	//			options.SetOptimizationLevel(shaderc_optimization_level_performance);

	//		//shaderc_spvc::

	//		const std::string& source = m_shaderSources.at(GL_VERTEX_SHADER);
	//		std::string name = m_name + " [VERTEX]";
	//		shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, shaderc_vertex_shader, name.c_str(), options);

	//		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
	//		{
	//			Log::cometError(result.GetErrorMessage());
	//			CMT_COMET_ASSERT(false);
	//		}

	//		openGLBinaries[0] = std::vector<uint32_t>(result.cbegin(), result.cend());

	//		//Cache compiled binary
	//		std::filesystem::path path = m_filepath;
	//		std::filesystem::path cachedDirectoryPath = path.parent_path() / "cached";
	//		path = cachedDirectoryPath / (m_name + ".vulkanBinary.vert");
	//		std::string cachedBinaryPath = path.string();

	//		//Create 'cached' directory if it does not exist
	//		std::filesystem::create_directory(cachedDirectoryPath);

	//		std::ofstream output(cachedBinaryPath, std::ios::binary);
	//		if (!output)
	//		{
	//			Log::cometError("Cannot create file to cache shader binary at {0}", cachedBinaryPath);
	//			CMT_COMET_ASSERT(false);
	//		}

	//		output.write((char*)vulkanBinaries[0].data(), vulkanBinaries[0].size() * sizeof(uint32_t));
	//		output.close();
	//	}

	//	//Fragment
	//	if (vulkanBinaries[1].size() == 0)
	//	{
	//		//Compile binary
	//		shaderc::Compiler compiler;
	//		shaderc::CompileOptions options;
	//		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

	//		if (optimisation)
	//			options.SetOptimizationLevel(shaderc_optimization_level_performance);

	//		const std::string& source = m_shaderSources.at(GL_FRAGMENT_SHADER);
	//		std::string name = m_name + " [FRAGMENT]";
	//		shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, shaderc_fragment_shader, name.c_str(), options);

	//		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
	//		{
	//			Log::cometError(result.GetErrorMessage());
	//			CMT_COMET_ASSERT(false);
	//		}

	//		openGLBinaries[1] = std::vector<uint32_t>(result.cbegin(), result.cend());

	//		//Cache compiled binary
	//		std::filesystem::path path = m_filepath;
	//		std::filesystem::path cachedDirectoryPath = path.parent_path() / "cached";
	//		path = cachedDirectoryPath / (m_name + ".vulkanBinary.frag");
	//		std::string cachedBinaryPath = path.string();

	//		//Create 'cached' directory if it does not exist
	//		std::filesystem::create_directory(cachedDirectoryPath);

	//		std::ofstream output(cachedBinaryPath, std::ios::binary);
	//		if (!output)
	//		{
	//			Log::cometError("Cannot create file to cache shader binary at {0}", cachedBinaryPath);
	//			CMT_COMET_ASSERT(false);
	//		}

	//		output.write((char*)vulkanBinaries[1].data(), vulkanBinaries[1].size() * sizeof(uint32_t));
	//		output.close();
	//	}
	//}

}
