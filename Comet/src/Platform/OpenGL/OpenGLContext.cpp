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
		CMT_COMET_ASSERT_MESSAGE(status, "Could not initialise Glad");
	}

	void Comet::OpenGLContext::swapBuffers() const
	{
		glfwSwapBuffers(m_windowHandle);
	}

	void OpenGLContext::onResize(uint32_t width, uint32_t height) const
	{
		glViewport(0, 0, width, height);
	}

}
