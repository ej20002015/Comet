#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/RenderingContext.h"

#include "GLFW/glfw3.h"

namespace Comet
{

class OpenGLContext : public RenderingContext
{
public:
	OpenGLContext(GLFWwindow* const windowHandle);
		
	void swapBuffers() const override;
	void onResize(const uint32_t width, const uint32_t height) const override;

private:
	GLFWwindow* const m_windowHandle;
};

}