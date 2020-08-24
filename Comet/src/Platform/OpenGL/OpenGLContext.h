#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/RenderingContext.h"

#include "GLFW/glfw3.h"

namespace Comet
{

	class OpenGLContext : public RenderingContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		
		void swapBuffers() override;

	private:
		GLFWwindow* m_windowHandle;
	};

}