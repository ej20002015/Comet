#pragma once
#include "CometPCH.h"

#include "Comet/Core/Core.h"

#include "spdlog/fmt/ostr.h"

namespace Comet
{

//Normally would use the all uppercase underscore naming method but these EventType Enum names need to be the same as their
//corresponding event classes
enum class EventType
{
	None = 0,
	WindowResizedEvent, WindowClosedEvent, WindowFocusedEvent, WindowLostFocusEvent, WindowMovedEvent,
	KeyPressedEvent, KeyReleasedEvent, KeyTypedEvent,
	MouseMovedEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseScrolledEvent
};


//Need to implicitly cast to an int for bitwise operations - as such, a normal enum is used rather than an enum class
enum EventCategory
{
	EVENT_CATEGORY_NONE           = 0,
	EVENT_CATEGORY_APPLICATION    = CMT_BIT(0),
	EVENT_CATEGORY_INPUT          = CMT_BIT(1),
	EVENT_CATEGORY_KEYBOARD       = CMT_BIT(2),
	EVENT_CATEGORY_MOUSE          = CMT_BIT(3),
	EVENT_CATEGORY_MOUSE_BUTTON   = CMT_BIT(4)
};

class Event
{
public:

	bool handled = false;

	virtual const char* getEventName() const = 0;
	virtual EventType getEventType() const = 0;
	virtual int getEventCategory() const = 0;
	virtual std::string toString() const { return getEventName(); }

	bool isInEventCategory(const EventCategory eventCategory) { return eventCategory & getEventCategory(); }
};

#define CMT_EVENT_CLASS_TYPE(type)\
static  EventType getStaticEventType() { return EventType::type; }\
EventType getEventType() const override { return getStaticEventType(); }\
const char* getEventName() const override { return #type; }

#define CMT_EVENT_CLASS_CATEGORY(category)\
int getEventCategory() const override { return category; }

class EventDispatcher
{
public:
	EventDispatcher(Event& e) : m_event(e) {}

	//Function is a lamda function that wraps the callback function to execute and F will be infered by the compiler
	//Returns a bool indicating whether the current event matches the event belonging to the dispatcher
	template<typename T, typename F>
	bool dispatch(const F& function)
	{
		if (m_event.getEventType() == T::getStaticEventType())
		{
			m_event.handled = function(static_cast<T&>(m_event));
			return true;
		}
			
		return false;
	}

private:
	Event& m_event;
};

//For printing using output streams
inline std::ostream& operator <<(std::ostream& os, const Event& e) { return os << e.toString(); }

}