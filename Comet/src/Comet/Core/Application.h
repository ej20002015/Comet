#pragma once

#include "CometPCH.h"

#include "Window.h"
#include "Comet/Events/ApplicationEvent.h"
#include "Layer.h"
#include "LayerStack.h"

namespace Comet
{

	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void run();

		virtual void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popOverlay(Layer* overlay);

	private:
		bool onWindowClosed(WindowClosedEvent& e);

	private:
		std::unique_ptr<Window> m_window;
		LayerStack m_layerStack;
		bool m_running;
	};

	//TO BE DEFINED BY CLIENT PROGRAMS
	Application* getApplicationInstance();

}