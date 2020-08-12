#include "CometPCH.h"
#include "Application.h"

namespace Comet
{

	Application::Application()
		: m_running(true)
	{
		m_window = std::unique_ptr<Window>(Window::create());
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (m_running)
			m_window->onUpdate();
	}

}