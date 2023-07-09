#include "CometPCH.h"
#include "SpirvTools.h"

#include <regex>

namespace Comet
{

using namespace std::string_view_literals;
using namespace std::string_literals;

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
		if (SPIRType.columns == 1)
		{
			switch (SPIRType.vecsize)
			{
			case 1: return ShaderDataType::FLOAT;
			case 2: return ShaderDataType::FLOAT2;
			case 3: return ShaderDataType::FLOAT3;
			case 4: return ShaderDataType::FLOAT4;
			}
		}
		else if (SPIRType.columns == 3)
			return ShaderDataType::MAT3;
		else if (SPIRType.columns == 4)
			return ShaderDataType::MAT4;
			
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown SPIRType");
		return ShaderDataType::NONE;

	case spirv_cross::SPIRType::Int:
		if (SPIRType.vecsize == 1)
			return ShaderDataType::INT;
		else if (SPIRType.vecsize == 2)
			return ShaderDataType::INT2;
		else if (SPIRType.vecsize == 3)
			return ShaderDataType::INT3; 
		else if (SPIRType.vecsize == 4)
			return ShaderDataType::INT4;

		CMT_COMET_ASSERT_MESSAGE(false, "Unknown SPIRType");
		return ShaderDataType::NONE;

	case spirv_cross::SPIRType::UInt:
		if (SPIRType.vecsize == 1)
			return ShaderDataType::UINT;

		CMT_COMET_ASSERT_MESSAGE(false, "Unknown SPIRType");
		return ShaderDataType::NONE;
		break;

	case spirv_cross::SPIRType::Boolean:
		return ShaderDataType::BOOL;

	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown SPIRType");
		return ShaderDataType::NONE;
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

std::pair<std::string, uint32_t> SpirvTools::getOpenGLFromBinary(const std::vector<uint32_t> binary, const bool autoSetUniformLocations, const uint32_t locationBase)
{
	spirv_cross::CompilerGLSL compiler(binary);
	spirv_cross::CompilerGLSL::Options options;
	const std::string source = compiler.compile();

	if (autoSetUniformLocations)
		return insertUniformLocations(source, locationBase);

	return { source, 0 };
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
	const std::string spirvName = fmt::format("{0} [{1}]", name, shaderTypeToString(type));
	shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(shaderSource.data(), getShadercType(type), spirvName.c_str(), options);

	if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		throw CometException() << fmt::format("Compilation Error: {0}", result.GetErrorMessage());

	binary.insert(binary.begin(), result.cbegin(), result.cend());

	//Cache compiled binary

	const std::filesystem::path cachedDirectoryPath = sourcePath.parent_path();
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

		//Populate uniform struct with uniforms
		std::vector<UniformBufferElementDescriptor> members = reflectMembers(compiler, bufferType);
		std::ranges::for_each(members, [&uniformBuffer](const UniformBufferElementDescriptor& desc) { uniformBuffer.push(desc); });
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
		std::vector<UniformBufferElementDescriptor> members = reflectMembers(compiler, structType);
		std::ranges::for_each(members, [&uniformStruct](const UniformBufferElementDescriptor& desc) { uniformStruct.push(static_cast<UniformElementDescriptor>(desc)); });
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

std::vector<UniformBufferElementDescriptor> SpirvTools::reflectMembers(const spirv_cross::CompilerGLSL& compiler, spirv_cross::SPIRType structType, const std::string_view namePrefix, const uint32_t offsetStart)
{
	std::vector<UniformBufferElementDescriptor> members;

	const uint32_t memberCount = static_cast<uint32_t>(structType.member_types.size());
	for (uint32_t i = 0; i < memberCount; ++i)
	{
		const spirv_cross::SPIRType& memberType = compiler.get_type(structType.member_types[i]);
		const std::string memberName = (namePrefix.size() ? std::string(namePrefix) + "."s : "") + compiler.get_member_name(structType.self, i);
		const uint32_t memberOffset = offsetStart + compiler.type_struct_member_offset(structType, i);
		if (memberType.basetype == spirv_cross::SPIRType::Struct)
		{
			// Recursively get UnformDescriptors for members of nested structs
			const uint32_t nestedStructOffsetStart = offsetStart + memberOffset;
			const std::vector<UniformBufferElementDescriptor> nestedStructMembers = reflectMembers(compiler, memberType, memberName, nestedStructOffsetStart);
			members.insert(members.end(), nestedStructMembers.begin(), nestedStructMembers.end());
			continue;
		}

		const ShaderDataType memberShaderDataType = getShaderDataTypeFromSPIRType(memberType);
		const uint32_t memberSize = static_cast<uint32_t>(compiler.get_declared_struct_member_size(structType, i));

		const UniformBufferElementDescriptor element(memberName, memberShaderDataType, memberSize, memberOffset);
		members.push_back(element);

		Log::cometInfo("Created uniform element (member) declaration with name '{0}', type '{1}', size {2}, offset {3}", memberName, getShaderDataTypeString(memberShaderDataType), memberSize, memberOffset);
	}

	return members;
}

std::pair<std::string, uint32_t> SpirvTools::insertUniformLocations(std::string shaderSource, const uint32_t locationBase)
{
	static constexpr std::string_view freeFloatingUniformRegex = "^(?!.*layout.*uniform).*uniform"sv;

	uint32_t location = locationBase;
	const std::regex regex(freeFloatingUniformRegex.data());
	
	for (std::smatch match; std::regex_search(shaderSource, match, regex); )
		shaderSource.insert(match.position(), fmt::format("layout(location = {}) ", location++));

	return { shaderSource, location };
}

}
