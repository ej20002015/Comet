#include "CometPCH.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Comet
{

	Unique<RendererAPI> RendererAPI::s_instance = nullptr;
	//TODO: TEMPORARILY SETTING GRAPHICS API TYPE HERE - NEEDS TO BE SET BEFORE WINDOW IS CREATED BUT INIT NOT CALLED YET BECAUSE NEEDS GRAPHIC CONTEXT
	RendererAPIType RendererAPI::s_currentRendererAPIType = RendererAPIType::OPENGL;
	RendererAPICapabilities RendererAPI::s_rendererAPICapabilities = RendererAPICapabilities();

	void RendererAPI::init()
	{
		switch (s_currentRendererAPIType)
		{
			case RendererAPIType::OPENGL:
			{
				RendererAPI::s_instance = createUnique<OpenGLRendererAPI>();
				s_instance->i_init();
				break;
			}
			default:
			{
				CMT_COMET_ASSERT(false, "Unknown GraphicsAPI");
				break;
			}
		}
	}

	void RendererAPI::shutdown()
	{
		CMT_COMET_ASSERT(s_instance, "Need to initialise the RendererAPI class first by calling init()");
		s_instance->i_shutdown();
	}

	void RendererAPI::setClearColor(float r, float g, float b, float a)
	{
		CMT_COMET_ASSERT(s_instance, "Need to initialise the RendererAPI class first by calling init()");
		s_instance->i_setClearColor(r, g, b, a);
	}

	const RendererAPICapabilities& RendererAPI::getCapabilities()
	{
		CMT_COMET_ASSERT(s_instance, "Need to initialise the RendererAPI class first by calling init()");
		return s_rendererAPICapabilities;
	}

}
