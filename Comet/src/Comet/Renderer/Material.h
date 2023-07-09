#pragma once
#include "CometPCH.h"

#include "Texture.h"

namespace Comet
{

struct Material
{
	static Reference<Material> create() { return createReference<Material>(); }

	glm::vec4 baseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	float roughness = 1.0f;
	float metalness = 1.0f;
	Reference<const Texture> baseColorMap;
	Reference<const Texture> roughnessMap;
	Reference<const Texture> metalnessMap;
	Reference<const Texture> normalMap;
};

}
