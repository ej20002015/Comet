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
		Application(const std::string_view windowTitle = "Comet Application");
		virtual ~Application();

		Application(const Application&) = delete;
		
		void initialiseWindow(const std::string_view windowTitle);

		static Application& get() { return *s_instance; }

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popOverlay(Layer* overlay);

		void exit() { m_running = false; }

		const Window& getWindow() const { return *m_window; }

		ImGuiLayer& getImGuiLayer() { return *m_ImGuiLayer; }

	private:
		void run();
		void renderImGui();
		void updateLayers();
		void updateTimestep();
		bool onWindowClosedEvent(WindowClosedEvent& e);
		bool onWindowResizedEvent(WindowResizedEvent& e);

	private:
		static Application* s_instance;
		Unique<Window> m_window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_layerStack;
		bool m_running;
		float m_timeAtLastFrame = 0.0f;
		Timestep m_ts;

	private:
		friend int ::main(int argc, char** argv);
	};

	//TO BE DEFINED BY CLIENT PROGRAMS
	Application* getApplicationInstance();

}