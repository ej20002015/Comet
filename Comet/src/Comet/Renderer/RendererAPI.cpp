#include "CometPCH.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Comet
{

Unique<RendererAPI> RendererAPI::s_instance = nullptr;
RendererAPI::Capabilities RendererAPI::s_rendererAPICapabilities = RendererAPI::Capabilities();

void RendererAPI::init()
{
	switch (s_currentRendererAPIType)
	{
		case Type::OPENGL:
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

void RendererAPI::drawIndexed(const uint32_t count, const PrimitiveType primitive)
{
	CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
	s_instance->i_drawIndexed(count, primitive);
}

void RendererAPI::drawIndexedFromVertexOffset(uint32_t count, const void* startOfIndices, uint32_t vertexOffset)
{
	CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
	s_instance->i_drawIndexedFromVertexOffset(count, startOfIndices, vertexOffset);
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

bool RendererAPI::getBackfaceCulling()
{
	CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
	return s_instance->i_getBackfaceCulling();
}

void RendererAPI::setBackfaceCulling(const bool culling)
{
	CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
	s_instance->i_setBackfaceCulling(culling);
}

bool RendererAPI::getDepthTesting()
{
	CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
	return s_instance->i_getDepthTesting();
}

void RendererAPI::setDepthTesting(const bool isDepthTestingEnabled)
{
	CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
	s_instance->i_setDepthTesting(isDepthTestingEnabled);
}

const RendererAPI::Capabilities& RendererAPI::getCapabilities()
{
	CMT_COMET_ASSERT_MESSAGE(s_instance, "Need to initialise the RendererAPI class first by calling init()");
	return s_rendererAPICapabilities;
}

}
