#include "CometPCH.h"
#include "SpirvTools.h"

#include "spvc/spvc.hpp"

namespace Comet
{

static std::string getShaderEnvironmentExtension(const ShaderEnvironment type)
{
	switch (type)
	{
	case ShaderEnvironment::VULKAN:   return ".VulkanBinary";  break;
	case ShaderEnvironment::OPENGL:   return ".OpenGLBinary"; break;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown shader environment");
		return "UNKNOWN";
		break;
	}
}

static std::string getShaderTypeExtension(const ShaderType type)
{
	switch (type)
	{
		case ShaderType::VERTEX:     return ".vert";  break;
		case ShaderType::FRAGMENT:   return ".frag"; break;
		case ShaderType::COMPUTE:    return ".comp"; break;
		default:
			CMT_COMET_ASSERT_MESSAGE(false, "Unknown shader type");
			return "UNKNOWN";
			break;
	}
}

static shaderc_shader_kind getShadercType(const ShaderType type)
{
	switch (type)
	{
		case ShaderType::VERTEX:     return shaderc_shader_kind::shaderc_vertex_shader;  break;
		case ShaderType::FRAGMENT:   return shaderc_shader_kind::shaderc_fragment_shader;  break;
		case ShaderType::COMPUTE:    return shaderc_shader_kind::shaderc_compute_shader;  break;
		default:
			CMT_COMET_ASSERT_MESSAGE(false, "Unknown shader type");
			return shaderc_glsl_infer_from_source;
			break;
	}
}

static std::string shaderTypeToString(const ShaderType type)
{
	switch (type)
	{
		case ShaderType::VERTEX:    return "VERTEX"; break;
		case ShaderType::FRAGMENT:  return "FRAGMENT"; break;
		case ShaderType::COMPUTE:   return "COMPUTE"; break;
		default:
			CMT_COMET_ASSERT_MESSAGE(false, "Unknown shader type");
			return "UNKNOWN";
			break;
	}
}

static ShaderDataType getShaderDataTypeFromSPIRType(const spirv_cross::SPIRType& SPIRType)
{
	switch (SPIRType.basetype)
	{
	case spirv_cross::SPIRType::Float:
		if (SPIRType.vecsize == 1)
			return ShaderDataType::FLOAT;
		else if (SPIRType.vecsize == 2)
			return ShaderDataType::FLOAT2;
		else if (SPIRType.vecsize == 3)
			return ShaderDataType::FLOAT3;
		else if (SPIRType.vecsize == 4)
			return ShaderDataType::FLOAT4;
		else if (SPIRType.columns == 3)
			return ShaderDataType::MAT3;
		else if (SPIRType.columns == 4)
			return ShaderDataType::MAT4;
		else
		{
			CMT_COMET_ASSERT_MESSAGE(false, "Unknown SPIRType");
			return ShaderDataType::NONE;
		}
		break;

	case spirv_cross::SPIRType::Int:
		if (SPIRType.vecsize == 1)
			return ShaderDataType::INT;
		else if (SPIRType.vecsize == 2)
			return ShaderDataType::INT2;
		else if (SPIRType.vecsize == 3)
			return ShaderDataType::INT3; 
		else if (SPIRType.vecsize == 4)
			return ShaderDataType::INT4;
		else
		{
			CMT_COMET_ASSERT_MESSAGE(false, "Unknown SPIRType");
			return ShaderDataType::NONE;
		}
		break;

	case spirv_cross::SPIRType::Boolean:
		return ShaderDataType::BOOL;
		break;

	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown SPIRType");
		return ShaderDataType::NONE;
		break;
	}
}

SpirvShaderInformation SpirvTools::compileAndReflect(const std::string_view shaderSource, const std::filesystem::path& sourcePath, const std::string_view name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimisation)
{
	CMT_COMET_ASSERT_MESSAGE(type != ShaderType::COMPUTE, "Compute shaders not yet supported");

    SpirvShaderInformation shaderInfo;

	compileOrRetrieveBinary(shaderInfo.binary, shaderSource, sourcePath, name, type, shaderEnvironment, optimisation);
	reflect(shaderInfo);
		
	return shaderInfo;
}

std::vector<uint32_t> SpirvTools::compile(const std::string_view shaderSource, const std::filesystem::path& sourcePath, const std::string_view name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimisation)
{
	CMT_COMET_ASSERT_MESSAGE(type != ShaderType::COMPUTE, "Compute shaders not yet supported");

	std::vector<uint32_t> binary;
	compileOrRetrieveBinary(binary, shaderSource, sourcePath, name, type, shaderEnvironment, optimisation);
	return binary;
}

std::string SpirvTools::getOpenGLFromBinary(const std::vector<uint32_t> binary)
{
	spirv_cross::CompilerGLSL compiler(binary);
	spirv_cross::CompilerGLSL::Options options;
	options.version = 450;
	options.es = false;
	compiler.set_common_options(options);

	return compiler.compile();
}

void SpirvTools::compileOrRetrieveBinary(std::vector<uint32_t>& binary, const std::string_view shaderSource, const std::filesystem::path& sourcePath, const std::string_view name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimisation)
{
	//TODO: Incorporate SPIRV-Tools and use spirv-opt to enable optimisation

	//See if spirv binaries have already been generated for this shader, and if so retrieve them

	const std::filesystem::path cachedBinaryPath = getCachedBinaryPath(sourcePath, name, type, shaderEnvironment);

	std::ifstream input(cachedBinaryPath, std::ios::binary);
	if (input)
	{
		Log::cometInfo("Retrieving cached shader at {0}", cachedBinaryPath.string());
		input.seekg(0, input.end);
		uint64_t size = static_cast<uint64_t>(input.tellg());
		input.seekg(0, input.beg);

		binary.resize(size / sizeof(uint32_t));
		input.read(reinterpret_cast<char*>(binary.data()), size);
		input.close();

		return;
	}

	//Cached binaries have not be found so need to compile the shader

	Log::cometInfo("Compiling and caching shader {0}", name);
	const shaderc::Compiler compiler;
	const shaderc::CompileOptions options = getCompileOptions(shaderEnvironment);
	std::string spirvName = fmt::format("{0} [{1}]", name, shaderTypeToString(type));
	shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(shaderSource.data(), getShadercType(type), spirvName.c_str(), options);

	if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		throw CometException() << fmt::format("Compilation Error: {0}", result.GetErrorMessage());

	binary.insert(binary.begin(), result.cbegin(), result.cend());

	//Cache compiled binary

	std::filesystem::path cachedDirectoryPath = sourcePath.parent_path();
	std::filesystem::create_directory(cachedDirectoryPath);

	std::ofstream output(cachedBinaryPath, std::ios::binary);
	if (!output)
		throw CometException() << fmt::format("Cannot create file '{0}' to cache '{1}' shader binary ", cachedDirectoryPath.string(), name);

	output.write(reinterpret_cast<char*>(binary.data()), binary.size() * sizeof(uint32_t));
	output.close();
}

std::filesystem::path SpirvTools::getCachedBinaryPath(const std::filesystem::path& sourcePath, const std::string_view name, const ShaderType type, const ShaderEnvironment shaderEnvironment)
{
	std::stringstream ss;
	ss << name << getShaderEnvironmentExtension(shaderEnvironment) << getShaderTypeExtension(type);
	const std::string cachedBinaryFilename = ss.str();
	const std::filesystem::path cachedBinaryPath = sourcePath.parent_path() /
												   "cached" /
												   cachedBinaryFilename;
		
	return cachedBinaryPath;
}

shaderc::CompileOptions SpirvTools::getCompileOptions(const ShaderEnvironment shaderEnvironment)
{
	shaderc::CompileOptions options;
	switch (shaderEnvironment)
	{
	case ShaderEnvironment::OPENGL:
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		break;
	case ShaderEnvironment::VULKAN:
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		break;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown target environment");
		break;
	}

	return options;
}

void SpirvTools::reflect(SpirvShaderInformation& shaderInfo)
{
	//Set up reflection

	CMT_COMET_ASSERT_MESSAGE(shaderInfo.binary.size(), "Binary recieved is empty");

	spirv_cross::CompilerGLSL compiler(shaderInfo.binary);

	// The SPIR-V is now parsed, and we can perform reflection on it.
	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	reflectUniformBuffers(shaderInfo, compiler, resources);
	reflectPushConstants(shaderInfo, compiler, resources);
	reflectSamplers(shaderInfo, compiler, resources);
}

void SpirvTools::reflectUniformBuffers(SpirvShaderInformation& shaderInfo, const spirv_cross::CompilerGLSL& compiler, const spirv_cross::ShaderResources& resources)
{
	for (auto& resource : resources.uniform_buffers)
	{
		const spirv_cross::SPIRType& bufferType = compiler.get_type(resource.base_type_id);
		std::string bufferName = resource.name;
		uint32_t bufferSize = static_cast<uint32_t>(compiler.get_declared_struct_size(bufferType));
		uint32_t bufferBindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);
		shaderInfo.uniformBuffers.emplace_back(bufferName, bufferSize, bufferBindingPoint);
		UniformBufferDescriptor& uniformBuffer = shaderInfo.uniformBuffers.back();

		Log::cometInfo("Created uniform buffer declaration with name '{0}', size {1} bytes, bindingPoint {2}", bufferName, bufferSize, bufferBindingPoint);

		//Populate uniform buffer with uniforms
		uint32_t elementCount = static_cast<uint32_t>(bufferType.member_types.size());
		uniformBuffer.reserve(elementCount);
		for (uint32_t i = 0; i < elementCount; ++i)
		{
			const spirv_cross::SPIRType& uniformType = compiler.get_type(bufferType.member_types[i]);
			std::string uniformName = compiler.get_member_name(bufferType.self, i);
			ShaderDataType shaderUniformType = getShaderDataTypeFromSPIRType(uniformType);
			uint32_t uniformSize = static_cast<uint32_t>(compiler.get_declared_struct_member_size(bufferType, i));
			uint32_t uniformOffset = compiler.type_struct_member_offset(bufferType, i);

			UniformBufferElementDescriptor element(uniformName, shaderUniformType, uniformSize, uniformOffset);
			uniformBuffer.push(element);

			Log::cometInfo("Created uniform buffer element declaration with name '{0}', type '{1}', size {2}, offset {3}", uniformName, getShaderDataTypeString(shaderUniformType), uniformSize, uniformOffset);
		}
	}
}

void SpirvTools::reflectPushConstants(SpirvShaderInformation& shaderInfo, const spirv_cross::CompilerGLSL& compiler, const spirv_cross::ShaderResources& resources)
{
	for (auto& resource : resources.push_constant_buffers)
	{
		const spirv_cross::SPIRType& structType = compiler.get_type(resource.base_type_id);
		std::string structName = resource.name;
		uint32_t structSize = static_cast<uint32_t>(compiler.get_declared_struct_size(structType));
		shaderInfo.uniformStructs.emplace_back(structName, structSize);
		UniformStructDescriptor& uniformStruct = shaderInfo.uniformStructs.back();

		Log::cometInfo("Created uniform struct declaration with name '{0}', size {1} bytes", structName, structSize);

		//Populate uniform struct with uniforms
		uint32_t elementCount = static_cast<uint32_t>(structType.member_types.size());
		uniformStruct.reserve(elementCount);
		for (uint32_t i = 0; i < elementCount; ++i)
		{
			const spirv_cross::SPIRType& uniformType = compiler.get_type(structType.member_types[i]);
			std::string uniformName = compiler.get_member_name(structType.self, i);
			ShaderDataType shaderUniformType = getShaderDataTypeFromSPIRType(uniformType);
			uint32_t uniformSize = static_cast<uint32_t>(compiler.get_declared_struct_member_size(structType, i));
			uint32_t uniformOffset = compiler.type_struct_member_offset(structType, i);

			UniformDescriptor element(uniformName, shaderUniformType, uniformSize);
			uniformStruct.push(element);

			Log::cometInfo("Created uniform buffer element declaration with name '{0}', type '{1}', size {2}, offset {3}", uniformName, getShaderDataTypeString(shaderUniformType), uniformSize, uniformOffset);
		}
	}
}

void SpirvTools::reflectSamplers(SpirvShaderInformation& shaderInfo, const spirv_cross::CompilerGLSL& compiler, const spirv_cross::ShaderResources& resources)
{
	for (auto& resource : resources.sampled_images)
	{
		const spirv_cross::SPIRType& samplerType = compiler.get_type(resource.base_type_id);
		uint32_t samplerDimension = samplerType.image.dim;
		std::string samplerName = resource.name;
		uint32_t samplerBindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);
		shaderInfo.uniformResources.emplace_back(samplerName, ShaderDataType::INT, 4, samplerBindingPoint);
	}
}

}
