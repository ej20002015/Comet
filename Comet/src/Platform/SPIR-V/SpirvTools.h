#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/UniformBufferDescriptor.h"
#include "Comet/Renderer/ShaderType.h"

#include "shaderc/shaderc.hpp"
#include "spirv_cross/spirv_glsl.hpp"

namespace Comet
{

enum class ShaderEnvironment
{
	OPENGL,
	VULKAN
};

struct SpirvShaderInformation
{
	std::vector<uint32_t> binary;
	std::vector<UniformBufferDescriptor> uniformBuffers;
	//OpenGL uniform structs go here, aswell as vulkan push constants
	std::vector<UniformStructDescriptor> uniformStructs;
	std::vector<UniformResourceDescriptor> uniformResources;
};

class SpirvTools
{
public:
	static SpirvShaderInformation compileAndReflect(const std::string_view shaderSource, const std::filesystem::path& sourcePath, const std::string_view name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimisation = false);
	static std::vector<uint32_t> compile(const std::string_view shaderSource, const std::filesystem::path& sourcePath, const std::string_view name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimisation = false);
	static std::string getOpenGLFromBinary(const std::vector<uint32_t> binary);

private:
	static void compileOrRetrieveBinary(std::vector<uint32_t>& binary, const std::string_view shaderSource, const std::filesystem::path& sourcePath, const std::string_view name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimisation);
	static std::filesystem::path getCachedBinaryPath(const std::filesystem::path& sourcePath, const std::string_view name, const ShaderType type, const ShaderEnvironment shaderEnvironment);
	static shaderc::CompileOptions getCompileOptions(const ShaderEnvironment shaderEnvironment);
	static void reflect(SpirvShaderInformation& shaderInfo);
	static void reflectUniformBuffers(SpirvShaderInformation& shaderInfo, const spirv_cross::CompilerGLSL& compiler, const spirv_cross::ShaderResources& resources);
	static void reflectPushConstants(SpirvShaderInformation& shaderInfo, const spirv_cross::CompilerGLSL& compiler, const spirv_cross::ShaderResources& resources);
	static void reflectSamplers(SpirvShaderInformation& shaderInfo, const spirv_cross::CompilerGLSL& compiler, const spirv_cross::ShaderResources& resources);
};

}