#pragma once
#include "CometPCH.h"

namespace Comet
{
	
	enum class ShaderDataType
	{
		NONE, FLOAT, FLOAT2, FLOAT3, FLOAT4, MAT3, MAT4, UINT, INT, INT2, INT3, INT4, BOOL
	};

	uint32_t getShaderDataTypeSize(ShaderDataType type);

	uint32_t getShaderDataTypeComponentCount(ShaderDataType type);

	std::string getShaderDataTypeString(ShaderDataType type);

}