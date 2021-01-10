#pragma once
#include "CometPCH.h"

#include "Comet/Core/Window.h"
#include "Comet/Renderer/RenderingContext.h"

#include "GLFW/glfw3.h"

namespace Comet
{

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const Window::WindowProperties& properties);
		~WindowsWindow() override;

		void onUpdate(Timestep ts) override;

		uint32_t getWidth() const override { return m_windowData.width; };
		uint32_t getHeight() const override { return m_windowData.height; };

		void setEventCallback(const EventCallbackFunction& callbackFunction) override { m_windowData.callbackFunction = callbackFunction; }
		void setVSync(bool enabled) override;
		bool getVSync() const override { return m_windowData.VSync; }

		bool getWindowFocused() const override;

		float getWindowTime() const override { return static_cast<float>(glfwGetTime()); }

		void* getImplementationWindow() const override { return m_window; }
		const RenderingContext& getRenderingContext() const override{ return *m_context; }

	public:
		static uint8_t s_GLFWWindowCount;

	private:
		void init();
		void shutdown();

	private:
		GLFWwindow* m_window;
		Unique<RenderingContext> m_context;

		//Specific data required for the windowing implementation
		struct WindowData
		{
			std::string title;
			uint32_t width, height;
			bool VSync;

			EventCallbackFunction callbackFunction;
		};

		WindowData m_windowData;
	};

}