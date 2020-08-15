#include "CometPCH.h"
#include "Application.h"

#include "Comet/Events/Event.h"
#include "glad/glad.h"

namespace Comet
{

	Application* Application::s_instance = nullptr;

	Application::Application()
		: m_running(true)
	{
		CMT_COMET_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(CMT_BIND_EVENT_FUNCTION(Application::onEvent));
	}

	void Application::run()
	{
		while (m_running)
		{
			//TODO: TEMP
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (auto layer : m_layerStack)
				layer->onUpdate();

			m_window->onUpdate();
		}
	}

	void Application::onEvent(Event& e)
	{
		Log::cometInfo(e);

		EventDispatcher dispatcher(e);

		dispatcher.dispatch<WindowClosedEvent>(CMT_BIND_EVENT_FUNCTION(Application::onWindowClosedEvent));

		//Propagate events down the layer stack in reverse order (from the top item on the stack downwards - overlays to layers)
		for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it++)
		{
			(*it)->onEvent(e);
			//If event has been handled then do not pass it done to the other layers
			if (e.handled)
				break;
		}
	}

	void Application::pushLayer(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
	}

	void Application::popLayer(Layer* layer)
	{
		m_layerStack.popLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay)
	{
		m_layerStack.pushOverlay(overlay);
	}

	void Application::popOverlay(Layer* overlay)
	{
		m_layerStack.popOverlay(overlay);
	}

	bool Application::onWindowClosedEvent(WindowClosedEvent& e)
	{
		m_running = false;
		return true;
	}

}