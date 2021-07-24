#include "CometPCH.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Comet
{

	static GLenum getGLPrimitveType(PrimitiveType primitive)
	{
		switch (primitive)
		{
			case Comet::PrimitiveType::TRIANGLES:   return GL_TRIANGLES; break;
			case Comet::PrimitiveType::LINES:       return GL_LINES; break;
			default:
				CMT_COMET_ASSERT_MESSAGE(false, "Unknown primitive type");
				return 0;
				break;
		}
	}

	static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				Log::cometError("[OpenGL Debug HIGH] {0}", message);
				CMT_COMET_ASSERT(false);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				Log::cometWarn("[OpenGL Debug MEDIUM] {0}", message);
				break;
			case GL_DEBUG_SEVERITY_LOW:
				Log::cometInfo("[OpenGL Debug LOW] {0}", message);
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				//Log::cometTrace("[OpenGL Debug NOTIFICATION] {0}", message);
				break;
		}
	}


	void OpenGLRendererAPI::i_init()
	{
		glDebugMessageCallback(OpenGLLogMessage, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glFrontFace(GL_CCW);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_MULTISAMPLE);
		glEnable(GL_STENCIL_TEST);

		//Fill in renderer capabilities

		s_rendererAPICapabilities.vendor = (const char*)glGetString(GL_VENDOR);
		s_rendererAPICapabilities.renderer = (const char*)glGetString(GL_RENDERER);
		s_rendererAPICapabilities.version = (const char*)glGetString(GL_VERSION);

		glGetIntegerv(GL_MAX_SAMPLES, &s_rendererAPICapabilities.maxSamples);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &s_rendererAPICapabilities.maxAnisotropy);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &s_rendererAPICapabilities.maxTextureUnits);

		GLenum error;
		while ((error = glGetError()) != GL_NO_ERROR)
			Log::cometError("OpenGL Error {0}", error);
	}

	void OpenGLRendererAPI::i_shutdown()
	{
	}

	void OpenGLRendererAPI::i_drawIndexed(uint32_t count, PrimitiveType primitive, bool depthTest)
	{
		if (!depthTest)
			glDisable(GL_DEPTH_TEST);

		glDrawElements(getGLPrimitveType(primitive), count, GL_UNSIGNED_INT, nullptr);

		if (!depthTest)
			glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::i_clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	bool OpenGLRendererAPI::i_getBackfaceCulling() const
	{
		bool culling;
		glGetBooleanv(GL_CULL_FACE, reinterpret_cast<GLboolean*>(&culling));
		return culling;
	}

	void OpenGLRendererAPI::i_setBackfaceCulling(bool culling)
	{
		culling ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	}

	void OpenGLRendererAPI::i_setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

}


