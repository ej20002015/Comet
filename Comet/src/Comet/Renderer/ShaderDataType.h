#pragma once
#include "CometPCH.h"

namespace Comet
{
	
	enum class ShaderDataType
	{
		None, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	uint32_t getShaderDataTypeSize(ShaderDataType type);

	uint32_t getShaderDataTypeComponentCount(ShaderDataType type);

}