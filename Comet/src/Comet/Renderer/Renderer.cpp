#include "CometPCH.h"
#include "Renderer.h"

#include "Renderer2D.h"

namespace Comet
{

	//TODO: TEMPORARILY SETTING GRAPHICS API TYPE HERE - NEEDS TO BE SET BEFORE WINDOW IS CREATED BUT INIT NOT CALLED YET BECAUSE NEEDS GRAPHIC CONTEXT
	RendererAPIType RendererAPI::s_currentRendererAPIType = RendererAPIType::OPENGL;

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

	void Renderer::setBackfaceCulling(bool culling)
	{
		RendererAPI::setBackfaceCulling(culling);
	}

	void Renderer::drawIndexed(uint32_t count, PrimitiveType primitive, bool depthTest)
	{
		RendererAPI::drawIndexed(count, primitive, depthTest);
	}

}
