#include "CometPCH.h"
#include "ShaderDataType.h"

namespace Comet
{

	uint32_t getShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:     return 4;
		case ShaderDataType::Float2:    return 2 * 4;
		case ShaderDataType::Float3:    return 3 * 4;
		case ShaderDataType::Float4:    return 4 * 4;
		case ShaderDataType::Mat3:      return 3 * 3 * 4;
		case ShaderDataType::Mat4:      return 4 * 4 * 4;
		case ShaderDataType::Int:       return 4;
		case ShaderDataType::Int2:      return 2 * 4;
		case ShaderDataType::Int3:      return 3 * 4;
		case ShaderDataType::Int4:      return 4 * 4;
		case ShaderDataType::Bool:      return 1;

		default:
			CMT_COMET_ASSERT_MESSAGE(false, "Unknown Shader Data Type");
			return 0;
		}
	}

	uint32_t getShaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:     return 1;
		case ShaderDataType::Float2:    return 2;
		case ShaderDataType::Float3:    return 3;
		case ShaderDataType::Float4:    return 4;
		case ShaderDataType::Mat3:      return 3 * 3;
		case ShaderDataType::Mat4:      return 4 * 4;
		case ShaderDataType::Int:       return 1;
		case ShaderDataType::Int2:      return 2;
		case ShaderDataType::Int3:      return 3;
		case ShaderDataType::Int4:      return 4;
		case ShaderDataType::Bool:      return 1;

		default:
			CMT_COMET_ASSERT_MESSAGE(false, "Unknown Shader Data Type");
			return 0;
		}
	}

}