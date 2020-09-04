#pragma once
#include "CometPCH.h"

#include "ShaderDataType.h"

namespace Comet
{

	struct UniformDeclaration
	{
		std::string name;
		ShaderDataType type;
		uint32_t offset;
	};

	class UniformBuffer
	{
		
	};

}