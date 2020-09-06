#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/UniformBuffer.h"

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


	//TODO: USING UNIFORM BUFFER OBJECTS CURRENTLY - PERHAPS HAVE SPIRV SPECIFIC DESCRIPTORS
	struct SpirvShaderInformation
	{
		std::vector<uint32_t> binary;
		std::vector<UniformBuffer> uniformBuffers;
		//OpenGL uniform structs go here, aswell as vulkan push constants
		std::vector<UniformStruct> uniformStructs;
		std::vector<UniformResource> uniformResources;
	};

	class SpirvTools
	{
	public:
		static SpirvShaderInformation compileAndReflect(const std::string& shaderSource, const std::string& sourcePath, const std::string& name, const ShaderType type, const ShaderEnvironment shaderEnvironment, const bool optimise = false);

	private:
		static void compileOrRetrieveBinary(SpirvShaderInformation& shaderInfo, const std::string& shaderSource, const std::string& sourcePath, const std::string& name, const ShaderType type, const bool optimise = false);
		static void reflectOpenGL(SpirvShaderInformation& shaderInfo);
		static void reflectVulkan(SpirvShaderInformation& shaderInfo);
	};

}