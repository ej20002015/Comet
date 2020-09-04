#include "CometPCH.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Comet
{

	Unique<RendererAPI> RendererAPI::s_instance = nullptr;
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
				CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
				break;
			}
		}
	}

	void RendererAPI::shutdown()
	{
		CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
		s_instance->i_shutdown();
	}

	void RendererAPI::drawIndexed(uint32_t count, PrimitiveType primitive, bool depthTest)
	{
		CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
		s_instance->i_drawIndexed(count, primitive, depthTest);
	}

	void RendererAPI::setClearColor(const glm::vec4& color)
	{
		CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
		s_instance->i_setClearColor(color);
	}

	void RendererAPI::clear()
	{
		CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
		s_instance->i_clear();
	}

	const RendererAPICapabilities& RendererAPI::getCapabilities()
	{
		CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
		return s_rendererAPICapabilities;
	}

}
