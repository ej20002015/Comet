#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

namespace Comet
{

struct PointLight
{
	static Reference<PointLight> create() { return createReference<PointLight>(); }

	float lightRadius = 10.0f;
	glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
	float luminousPower = 4.0f * glm::pi<float>() * glm::pi<float>();
};

}