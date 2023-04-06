#include "CometPCH.h"
#include "OpenGLContext.h"

#include "glad/glad.h"

namespace Comet
{

OpenGLContext::OpenGLContext(GLFWwindow* const windowHandle)
	: m_windowHandle(windowHandle)
{
	glfwMakeContextCurrent(windowHandle);
	const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	CMT_COMET_ASSERT_MESSAGE(status, "Could not initialise Glad");
}

void Comet::OpenGLContext::swapBuffers() const
{
	glfwSwapBuffers(m_windowHandle);
}

void OpenGLContext::onResize(const uint32_t width, const uint32_t height) const
{
	glViewport(0, 0, width, height);
}

}
