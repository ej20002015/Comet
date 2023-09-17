#pragma once
#include "CometPCH.h"

#include "Comet/Events/Event.h"
#include "Comet/Renderer/RenderingContext.h"

namespace Comet
{

class Window
{
public:

	struct WindowProperties
	{
		std::string_view title;
		uint32_t width = DEFAULT_WIDTH;
		uint32_t height = DEFAULT_HEIGHT;
	};


	using EventCallbackFunction = std::function<void(Event&)>;

	static Unique<Window> create(const WindowProperties& properties = WindowProperties());

	virtual ~Window() = default;

	virtual void onUpdate(const Timestep ts) = 0;

	virtual uint32_t getWidth() const = 0;
	virtual uint32_t getHeight() const = 0;

	virtual void setEventCallback(const EventCallbackFunction& callbackFunction) = 0;
	virtual void setVSync(const bool enabled) = 0;
	virtual bool getVSync() const = 0;

	virtual bool getFocused() const = 0;

	virtual float getTime() const = 0;

	virtual void* getImplementationWindow() const = 0;
	virtual const RenderingContext& getRenderingContext() const = 0;
};

}