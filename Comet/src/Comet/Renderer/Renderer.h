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

		static void setClearColor(const glm::vec4& color);
		static void clear();

		static bool getBackfaceCulling();
		static void setBackfaceCulling(bool culling);

		static void drawIndexed(uint32_t count, PrimitiveType primitive, bool depthTest = true);
	};

}