#include "CometPCH.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include "SceneRenderer.h"

namespace Comet
{

//TODO: TEMPORARILY SETTING GRAPHICS API TYPE HERE - NEEDS TO BE SET BEFORE WINDOW IS CREATED BUT INIT NOT CALLED YET BECAUSE NEEDS GRAPHIC CONTEXT
RendererAPI::Type RendererAPI::s_currentRendererAPIType = Type::OPENGL;

void Renderer::init()
{
	RendererAPI::init();
	SceneRenderer::init();
	Renderer2D::init();
}

void Renderer::shutdown()
{
	Renderer2D::shutdown();
	SceneRenderer::shutdown();
	RendererAPI::shutdown();
}

}
