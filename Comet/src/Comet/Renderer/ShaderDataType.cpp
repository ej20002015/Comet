#include "CometPCH.h"
#include "ShaderDataType.h"

namespace Comet
{

uint32_t getShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::FLOAT:     return 4;
	case ShaderDataType::FLOAT2:    return 2 * 4;
	case ShaderDataType::FLOAT3:    return 3 * 4;
	case ShaderDataType::FLOAT4:    return 4 * 4;
	case ShaderDataType::MAT3:      return 3 * 3 * 4;
	case ShaderDataType::MAT4:      return 4 * 4 * 4;
	case ShaderDataType::UINT:		return 4;
	case ShaderDataType::INT:       return 4;
	case ShaderDataType::INT2:      return 2 * 4;
	case ShaderDataType::INT3:      return 3 * 4;
	case ShaderDataType::INT4:      return 4 * 4;
	case ShaderDataType::BOOL:      return 1;

	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown Shader Data Type");
		return 0;
	}
}

uint32_t getShaderDataTypeComponentCount(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::FLOAT:     return 1;
	case ShaderDataType::FLOAT2:    return 2;
	case ShaderDataType::FLOAT3:    return 3;
	case ShaderDataType::FLOAT4:    return 4;
	case ShaderDataType::MAT3:      return 3 * 3;
	case ShaderDataType::MAT4:      return 4 * 4;
	case ShaderDataType::UINT:		return 1;
	case ShaderDataType::INT:       return 1;
	case ShaderDataType::INT2:      return 2;
	case ShaderDataType::INT3:      return 3;
	case ShaderDataType::INT4:      return 4;
	case ShaderDataType::BOOL:      return 1;

	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown Shader Data Type");
		return 0;
	}
}

std::string getShaderDataTypeString(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::FLOAT:     return "FLOAT";
	case ShaderDataType::FLOAT2:    return "FLOAT2";
	case ShaderDataType::FLOAT3:    return "FLOAT3";
	case ShaderDataType::FLOAT4:    return "FLOAT4";
	case ShaderDataType::MAT3:      return "MAT3";
	case ShaderDataType::MAT4:      return "MAT4";
	case ShaderDataType::UINT:		return "UINT";
	case ShaderDataType::INT:       return "INT";
	case ShaderDataType::INT2:      return "INT2";
	case ShaderDataType::INT3:      return "INT3";
	case ShaderDataType::INT4:      return "INT4";
	case ShaderDataType::BOOL:      return "BOOL";

	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown Shader Data Type");
		return "";
	}
}

}