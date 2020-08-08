#pragma once

#include "Event.h"

#include <sstream>

namespace Comet
{

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

		CMT_EVENT_CLASS_TYPE(WindowResizedEvent)
		CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_APPLICATION)

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizedEvent: (" << m_width << ", " << m_height << ")";
			return ss.str();
		}

		uint32_t getWidth() const { return m_width; }
		uint32_t getHeight() const { return m_height; }

	private:
		uint32_t m_width, m_height;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		CMT_EVENT_CLASS_TYPE(WindowClosedEvent)
		CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_APPLICATION)
	};

	class WindowFocusedEvent : public Event
	{
	public:
		WindowFocusedEvent() = default;

		CMT_EVENT_CLASS_TYPE(WindowFocusedEvent)
		CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_APPLICATION)
	};

	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() = default;

		CMT_EVENT_CLASS_TYPE(WindowLostFocusEvent)
		CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_APPLICATION)
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent() = default;

		CMT_EVENT_CLASS_TYPE(WindowMovedEvent)
		CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_APPLICATION)
	};

}