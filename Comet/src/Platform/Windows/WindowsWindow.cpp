#include "CometPCH.h"
#include "WindowsWindow.h"

#include "Comet/Events/Event.h"
#include "Comet/Events/ApplicationEvent.h"
#include "Comet/Events/KeyboardEvent.h"
#include "Comet/Events/MouseEvent.h"

#include "Comet/Core/KeyCodes.h"
#include "Comet/Core/MouseCodes.h"

#include "Comet/Renderer/RenderingContext.h"

namespace Comet
{
	//Counts the number of GLFW windows in order to init and terminate GLFW at the appropriate times
	uint8_t WindowsWindow::s_GLFWWindowCount = 0;
	
	static void glfwErrorCallback(int errorCode, const char* description)
	{
		Log::cometError("GLFW error {0}: {1}", errorCode, description);
	}

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

	void WindowsWindow::onUpdate(Timestep ts)
	{
		glfwPollEvents();
		m_context->swapBuffers();
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
			CMT_COMET_ASSERT_MESSAGE(success, "Could not initialise GLFW");
			s_GLFWWindowCount++;
		}

		m_window = glfwCreateWindow((int)m_windowData.width, (int)m_windowData.height, m_windowData.title.c_str(), nullptr, nullptr);

		//Create RenderingContext
		m_context = RenderingContext::create(m_window);

		//This data will be sent by glfw when any glfw callback function is called and can be modified
		glfwSetWindowUserPointer(m_window, &m_windowData);
		setVSync(true);

		//SET UP EVENT CALLBACKS FOR GLFW

		//Error callback
		glfwSetErrorCallback(glfwErrorCallback);
		
		//Application Events
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.width = width;
			windowData.height = height;

			WindowResizedEvent e(width, height);
			windowData.callbackFunction(e);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowClosedEvent e;
			windowData.callbackFunction(e);
		});

		glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused)
		{
			WindowData& windowData = *(WindowData*)(glfwGetWindowUserPointer(window));

			if (focused)
			{
				WindowFocusedEvent e;
				windowData.callbackFunction(e);
			}
			else
			{
				WindowLostFocusEvent e;
				windowData.callbackFunction(e);
			}
		});

		glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int x, int y)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowMovedEvent e(x, y);
			windowData.callbackFunction(e);
		});

		//Keyboard events
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			//Need to scope the code within the case blocks as all case statements share a scope
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(static_cast<KeyCode>(keyCode));
					windowData.callbackFunction(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(static_cast<KeyCode>(keyCode), true);
					windowData.callbackFunction(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(static_cast<KeyCode>(keyCode));
					windowData.callbackFunction(e);
					break;
				}
				default:
				{
					Log::cometError("Unknown key action");
					break;
				}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int unicodeKeyCode)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent e(static_cast<KeyCode>(unicodeKeyCode));
			windowData.callbackFunction(e);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int mouseKeyCode, int action, int mods)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(static_cast<MouseCode>(mouseKeyCode));
					windowData.callbackFunction(e);
					break;
				}

				//GLFW does not currently support mouse button repeat events

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(static_cast<MouseCode>(mouseKeyCode));
					windowData.callbackFunction(e);
					break;
				}

				default:
				{
					Log::cometError("Unknown key action");
					break;
				}
			}
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent e(static_cast<float>(xPos), static_cast<float>(yPos));
			windowData.callbackFunction(e);
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent e(static_cast<float>(xOffset), static_cast<float>(yOffset));
			windowData.callbackFunction(e);
		});
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