#include "CometPCH.h"
#include "WindowsWindow.h"

namespace Comet
{
	//Counts the number of GLFW windows in order to init and terminate GLFW at the appropriate times
	uint8_t WindowsWindow::s_GLFWWindowCount = 0;
	

	WindowsWindow::WindowsWindow(const Window::WindowProperties& properties)
	{
		m_windowData.title  = properties.title;
		m_windowData.width  = properties.width;
		m_windowData.height = properties.height;
		init();
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_windowData.VSync = enabled;
	}

	void WindowsWindow::init()
	{
		Log::cometInfo("Creating window: {0} ({1}, {2})", m_windowData.title, m_windowData.width, m_windowData.height);

		if (!s_GLFWWindowCount)
		{
			int success = glfwInit();
			CMT_COMET_ASSERT(success, "Could not initialise GLFW");
			s_GLFWWindowCount++;
		}

		m_window = glfwCreateWindow((int)m_windowData.width, (int)m_windowData.height, m_windowData.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		//This data will be sent by glfw when any glfw callback function is called and can be modified
		glfwSetWindowUserPointer(m_window, &m_windowData);
		setVSync(true);
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(m_window);

		s_GLFWWindowCount--;

		if (!s_GLFWWindowCount)
		{
			glfwTerminate();
		}
	}

}