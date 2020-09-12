#include "CometPCH.h"
#include "SpirvTools.h"

//Spirv files
#include "shaderc/shaderc.hpp"
#include "spvc/spvc.hpp"
#include "spirv_cross/spirv_glsl.hpp"

namespace Comet
{

	static std::string getShaderEnvironmentExtension(const ShaderEnvironment type)
	{
		switch (type)
		{
		case ShaderEnvironment::VULKAN:     return ".VulkanBinary";  break;
		case ShaderEnvironment::OPENGL:     return ".OpenGLBinary"; break;
		default:
			Log::cometError("Unkown shader environment");
			CMT_COMET_ASSERT(false);
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
				Log::cometError("Unkown shader type");
				CMT_COMET_ASSERT(false);
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
				Log::cometError("Unkown shader type");
				CMT_COMET_ASSERT(false);
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
				Log::cometError("Unkown shader type");
				CMT_COMET_ASSERT(false);
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

	SpirvShaderInformation SpirvTools::compileAndReflect(const std::string& shaderSource, const std::string& sourcePath, const std::string& name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimise)
    {
		CMT_COMET_ASSERT_MESSAGE(type != ShaderType::COMPUTE, "Compute shaders not yet supported");

        SpirvShaderInformation shaderInfo;

		compileOrRetrieveBinary(shaderInfo.binary, shaderSource, sourcePath, name, type, shaderEnvironment, optimise);
		reflect(shaderInfo);
		
		return shaderInfo;
    }

	std::vector<uint32_t> SpirvTools::compile(const std::string& shaderSource, const std::string& sourcePath, const std::string& name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimise)
	{
		CMT_COMET_ASSERT_MESSAGE(type != ShaderType::COMPUTE, "Compute shaders not yet supported");
		std::vector<uint32_t> binary;
		compileOrRetrieveBinary(binary, shaderSource, sourcePath, name, type, shaderEnvironment, optimise);
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

	void SpirvTools::compileOrRetrieveBinary(std::vector<uint32_t>& binary, const std::string& shaderSource, const std::string& sourcePath, const std::string& name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimise)
	{
		//See if spirv binaries have already been generated for this shader, and if so retrieve them

		std::filesystem::path path = sourcePath;
		path = path.parent_path() / "cached" / (name + getShaderEnvironmentExtension(shaderEnvironment) + getShaderTypeExtension(type));
		std::string cachedBinaryPath = path.string();

		std::ifstream input(cachedBinaryPath, std::ios::binary);
		if (input)
		{
			Log::cometInfo("Retrieving cached shader at {0}", cachedBinaryPath);
			input.seekg(0, input.end);
			uint64_t size = static_cast<uint64_t>(input.tellg());
			input.seekg(0, input.beg);

			binary = std::vector<uint32_t>(size / sizeof(uint32_t));
			input.read((char*)binary.data(), size);
			input.close();
		}
		else
		{
			//If cached vulkan binaries have not be found then create them

			Log::cometInfo("Compiling and caching shader {0}", name);
			//Compile binary
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			switch (shaderEnvironment)
			{
				case ShaderEnvironment::OPENGL:
					options.SetTargetEnvironment(shaderc_target_env_opengl_compat, shaderc_env_version_opengl_4_5);
					break;
				case ShaderEnvironment::VULKAN:
					options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
					break;
				default:
					CMT_COMET_ASSERT_MESSAGE(false, "Unknown target environment");
					break;
			}

			if (optimise)
				options.SetOptimizationLevel(shaderc_optimization_level_performance);

			std::string spirvName = name + " [" + shaderTypeToString(type) + "]";
			shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(shaderSource, getShadercType(type), spirvName.c_str(), options);

			if (result.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				Log::cometError(result.GetErrorMessage());
				CMT_COMET_ASSERT(false);
			}

			binary = std::vector<uint32_t>(result.cbegin(), result.cend());

			//Cache compiled binary
			//Create 'cached' directory if it does not exist
			std::filesystem::path cachedDirectoryPath = path.parent_path();
			std::filesystem::create_directory(cachedDirectoryPath);

			std::ofstream output(cachedBinaryPath, std::ios::binary);
			if (!output)
			{
				Log::cometError("Cannot create file to cache shader binary at {0}", cachedBinaryPath);
				CMT_COMET_ASSERT(false);
			}

			output.write((char*)binary.data(), binary.size() * sizeof(uint32_t));
			output.close();
		}
	}

	void SpirvTools::reflect(SpirvShaderInformation& shaderInfo)
	{
		//Set up reflection
		CMT_COMET_ASSERT_MESSAGE(shaderInfo.binary.size(), "Binary recieved is empty");

		spirv_cross::CompilerGLSL compiler(shaderInfo.binary);

		// The SPIR-V is now parsed, and we can perform reflection on it.
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		// Get all uniform buffers from the shader
		for (auto& resource : resources.uniform_buffers)
		{
			const spirv_cross::SPIRType& bufferType = compiler.get_type(resource.base_type_id);
			std::string bufferName = resource.name;
			uint32_t bufferSize = static_cast<uint32_t>(compiler.get_declared_struct_size(bufferType));
			uint32_t bufferBindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);
			shaderInfo.uniformBuffers.emplace_back(bufferName, bufferSize, bufferBindingPoint);
			UniformBufferDescriptor& uniformBuffer = shaderInfo.uniformBuffers.back();

			Log::cometTrace("Created uniform buffer declaration with name '{0}', size {1} bytes, bindingPoint {2}", bufferName, bufferSize, bufferBindingPoint);
			
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

				Log::cometTrace("Created uniform buffer element declaration with name '{0}', type '{1}', size {2}, offset {3}", uniformName, getShaderDataTypeString(shaderUniformType), uniformSize, uniformOffset);
			}
		}

		//Get all uniform structs/push constants from the shader
		for (auto& resource : resources.push_constant_buffers)
		{
			const spirv_cross::SPIRType& structType = compiler.get_type(resource.base_type_id);
			std::string structName = resource.name;
			uint32_t structSize = static_cast<uint32_t>(compiler.get_declared_struct_size(structType));
			shaderInfo.uniformStructs.emplace_back(structName, structSize);
			UniformStructDescriptor& uniformStruct = shaderInfo.uniformStructs.back();

			Log::cometTrace("Created uniform struct declaration with name '{0}', size {1} bytes", structName, structSize);

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

				Log::cometTrace("Created uniform buffer element declaration with name '{0}', type '{1}', size {2}, offset {3}", uniformName, getShaderDataTypeString(shaderUniformType), uniformSize, uniformOffset);
			}
		}
	}

}
