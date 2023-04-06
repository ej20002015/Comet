#pragma once
#include "CometPCH.h"

namespace Comet
{

enum class ShaderType
{
	VERTEX,
	FRAGMENT,
	COMPUTE
};

static const std::vector<std::string> SHADER_TYPE_STRS =
{
	"vertex",
	"fragment",
	"compute"
};

static const ShaderType strToShaderType(const std::string_view str)
{
	if (str == "vertex")
		return ShaderType::VERTEX;
	if (str == "fragment")
		return ShaderType::FRAGMENT;
	if (str == "compute")
		return ShaderType::COMPUTE;

	CMT_COMET_ASSERT_MESSAGE(false, fmt::format("'{0}' does not map to a known ShaderType", str));
	return ShaderType::VERTEX;
}

}