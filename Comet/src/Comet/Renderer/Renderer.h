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
	static void setBackfaceCulling(const bool culling);

	static void drawIndexed(const uint32_t count, const RendererAPI::PrimitiveType primitive, const bool depthTest = true);
};

}