#include "CometPCH.h"
#include "OpenGLContext.h"

#include "glad/glad.h"

namespace Comet
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		glfwMakeContextCurrent(windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CMT_COMET_ASSERT(status, "Could not initialise Glad");
	}

	void Comet::OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}
