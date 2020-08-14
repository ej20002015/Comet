#pragma once

#include "CometPCH.h"

#include "Comet/Events/Event.h"

namespace Comet
{

	class Window
	{
	public:

		struct WindowProperties
		{
			std::string title;
			uint32_t width, height;

			WindowProperties(std::string title = "Comet Application",
							uint32_t width     = 1280,
							uint32_t height    = 720)
							: title(title), width(width), height(height) {}
		};


		using EventCallbackFunction = std::function<void(Event&)>;

		static std::unique_ptr<Window> create(const WindowProperties& properties = WindowProperties());

		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFunction& callbackFunction) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool getVSync() const = 0;

		virtual void* getImplementationWindow() const = 0;
	};

}