#pragma once
#include "CometPCH.h"

#include "RendererAPI.h"

#include "glm/glm.hpp"

namespace Comet
{

class Renderer
{
public:
	Renderer() = delete;

	static void init();
	static void shutdown();
};

}