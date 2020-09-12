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
		
		void swapBuffers() const override;
		void onResize(uint32_t width, uint32_t height) const override;

	private:
		GLFWwindow* m_windowHandle;
	};

}