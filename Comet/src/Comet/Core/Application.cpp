#include "CometPCH.h"
#include "Application.h"

#include "Comet/Events/Event.h"
#include "glad/glad.h"
#include "Input.h"
#include "Comet/Renderer/Renderer.h"

//temp
#include "TriangleLayer.h"

namespace Comet
{

	Application* Application::s_instance = nullptr;

	Application::Application()
		: m_running(true)
	{
		CMT_COMET_ASSERT_MESSAGE(!s_instance, "Application already exists");
		s_instance = this;

		//Initialise window
		m_window = Unique<Window>(Window::create());
		m_window->setEventCallback(CMT_BIND_EVENT_FUNCTION(Application::onEvent));
		//m_window->setVSync(false);

		//Create ImGui Layer
		m_ImGuiLayer = new ImGuiLayer;
		pushOverlay(m_ImGuiLayer);

		//Initialise input
		Input::init();

		//Initialise renderer
		Renderer::init();
	}

	Application::~Application()
	{
		Renderer::shutdown();
	}

	void Application::run()
	{
		while (m_running)
		{
			//Calculate timestep
			float currentTime = m_window->getWindowTime();
			Timestep ts(currentTime - m_timeAtLastFrame);
			m_timeAtLastFrame = currentTime;

			Renderer::clear();

			for (auto layer : m_layerStack)
				layer->onUpdate(ts);

			//Render all the ImGui ui set up by the layers in the stack
			m_ImGuiLayer->begin();
			for (auto layer : m_layerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->end();

			m_window->onUpdate(ts);
		}
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.dispatch<WindowClosedEvent>(CMT_BIND_EVENT_FUNCTION(Application::onWindowClosedEvent));
		dispatcher.dispatch<WindowResizedEvent>(CMT_BIND_EVENT_FUNCTION(Application::onWindowResizedEvent));

		//Propagate events down the layer stack in reverse order (from the top item on the stack downwards - overlays to layers)
		for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it++)
		{
			(*it)->onEvent(e);
			//If event has been handled then do not pass it to the other layers
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

	bool Application::onWindowResizedEvent(WindowResizedEvent& e)
	{
		m_window->getRenderingContext().onResize(e.getWidth(), e.getHeight());
		
		//Resize framebuffers
		for (Reference<Framebuffer> framebuffer : FramebufferPool::getGlobalPool())
			framebuffer->resize(e.getWidth(), e.getHeight());	

		return false;
	}

}