#include "CometPCH.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Comet
{

	static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				Log::cometError("[OpenGL Debug HIGH] {0}", message);
				CMT_COMET_ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
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

		//unsigned int vao;
		//glGenVertexArrays(1, &vao);
		//glBindVertexArray(vao);

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

		GLenum error = glGetError();
		while (error != GL_NO_ERROR)
		{
			Log::cometError("OpenGL Error {0}", error);
			error = glGetError();
		}
	}

	void OpenGLRendererAPI::i_shutdown()
	{
	}

	void OpenGLRendererAPI::i_setClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

}


