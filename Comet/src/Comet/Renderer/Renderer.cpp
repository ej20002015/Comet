#include "CometPCH.h"
#include "Renderer.h"

#include "Renderer2D.h"

namespace Comet
{

//TODO: TEMPORARILY SETTING GRAPHICS API TYPE HERE - NEEDS TO BE SET BEFORE WINDOW IS CREATED BUT INIT NOT CALLED YET BECAUSE NEEDS GRAPHIC CONTEXT
RendererAPI::Type RendererAPI::s_currentRendererAPIType = Type::OPENGL;

void Renderer::init()
{
	RendererAPI::init();
	Renderer2D::init();
}

void Renderer::shutdown()
{
	Renderer2D::shutdown();
	RendererAPI::shutdown();
}

void Renderer::setClearColor(const glm::vec4& color)
{
	//TODO: RENDER COMMAND QUEUE
	RendererAPI::setClearColor(color);
}

void Renderer::clear()
{
	RendererAPI::clear();
}

bool Renderer::getBackfaceCulling()
{
	return RendererAPI::getBackfaceCulling();
}

void Renderer::setBackfaceCulling(const bool culling)
{
	RendererAPI::setBackfaceCulling(culling);
}

void Renderer::drawIndexed(const uint32_t count, const RendererAPI::PrimitiveType primitive, const bool depthTest)
{
	RendererAPI::drawIndexed(count, primitive, depthTest);
}

}
