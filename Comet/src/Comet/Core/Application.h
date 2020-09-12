#pragma once
#include "CometPCH.h"

#include "Window.h"
#include "Comet/Events/ApplicationEvent.h"
#include "Layer.h"
#include "Comet/ImGui/ImGuiLayer.h"
#include "LayerStack.h"

#include "Comet/Renderer/IndexBuffer.h"
#include "Comet/Renderer/VertexBuffer.h"
#include "Comet/Renderer/Pipeline.h"

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
		bool onWindowResizedEvent(WindowResizedEvent& e);

	private:
		static Application* s_instance;
		Unique<Window> m_window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_layerStack;
		bool m_running;

		Reference<Pipeline> m_pipeline;
		Reference<VertexBuffer> m_vb;
		Reference<IndexBuffer> m_ib;

	private:
		friend int ::main(int argc, char** argv);
	};

	//TO BE DEFINED BY CLIENT PROGRAMS
	Application* getApplicationInstance();

}