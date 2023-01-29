#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/UniformBufferDescriptor.h"

namespace Comet
{

	enum class ShaderEnvironment
	{
		OPENGL,
		VULKAN
	};

	enum class ShaderType
	{
		VERTEX,
		FRAGMENT,
		COMPUTE
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
		static SpirvShaderInformation compileAndReflect(const std::string& shaderSource, const std::filesystem::path& sourcePath, const std::string& name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimise = false);
		static std::vector<uint32_t> compile(const std::string& shaderSource, const std::filesystem::path& sourcePath, const std::string& name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimise = false);
		static std::string getOpenGLFromBinary(const std::vector<uint32_t> binary);

	private:
		static void compileOrRetrieveBinary(std::vector<uint32_t>& binary, const std::string& shaderSource, const std::filesystem::path& sourcePath, const std::string& name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimise = false);
		static void reflect(SpirvShaderInformation& shaderInfo);
	};

}