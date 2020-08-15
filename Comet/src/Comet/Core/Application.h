#pragma once

#include "CometPCH.h"

#include "Window.h"
#include "Comet/Events/ApplicationEvent.h"
#include "Layer.h"
#include "LayerStack.h"

int main(int argc, char** argv);

namespace Comet
{

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		Application(const Application&) = delete;

		static Application& get() { return *s_instance; }

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popOverlay(Layer* overlay);

		Window& getWindow() const { return *m_window; }

	private:
		void run();
		bool onWindowClosedEvent(WindowClosedEvent& e);

	private:
		static Application* s_instance;
		std::unique_ptr<Window> m_window;
		LayerStack m_layerStack;
		bool m_running;
		friend int ::main(int argc, char** argv);
	};

	//TO BE DEFINED BY CLIENT PROGRAMS
	Application* getApplicationInstance();

}