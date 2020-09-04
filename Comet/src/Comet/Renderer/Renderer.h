#pragma once
#include "CometPCH.h"

#include "RendererAPI.h"

#include "glm/glm.hpp"

namespace Comet
{

	class Renderer
	{
	public:
		static void init();

		static void setClearColor(const glm::vec4& color);
		static void clear();

		static void drawIndexed(uint32_t count, PrimitiveType primitive, bool depthTest = true);
	};

}