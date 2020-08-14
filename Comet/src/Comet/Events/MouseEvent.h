#pragma once

#include "Event.h"
#include "Comet/Core/Input.h"

#include <sstream>

namespace Comet
{

	class MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent(KeyCode keyCode) : m_keyCode(keyCode) {}

		CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_MOUSE_BUTTON | EventCategory::EVENT_CATEGORY_MOUSE| EventCategory::EVENT_CATEGORY_INPUT)

		KeyCode getKeyCode() const { return m_keyCode; }

	protected:
		KeyCode m_keyCode;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(KeyCode keyCode, int repeatCount = 0) : MouseButtonEvent(keyCode), m_repeatCount(repeatCount) {}

		CMT_EVENT_CLASS_TYPE(MouseButtonPressedEvent)

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

		int getRepeatCount() const { return m_repeatCount; }

	private:
		int m_repeatCount;
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(KeyCode keyCode) : MouseButtonEvent(keyCode) {}

		CMT_EVENT_CLASS_TYPE(MouseButtonReleasedEvent)

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_keyCode;
			return ss.str();
		}
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float mousePosX, float mousePosY) : m_mousePosX(mousePosX), m_mousePosY(mousePosY) {}

		CMT_EVENT_CLASS_TYPE(MouseMovedEvent)
		CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_MOUSE | EventCategory::EVENT_CATEGORY_INPUT)

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << m_mousePosX << ", " << m_mousePosY << ")";
			return ss.str();
		}

		float getMousePosX() const { return m_mousePosX; }
		float getMousePosY() const { return m_mousePosY; }

	private:
		float m_mousePosX, m_mousePosY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

		CMT_EVENT_CLASS_TYPE(MouseScrolledEvent)
		CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_MOUSE | EventCategory::EVENT_CATEGORY_INPUT)

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << m_xOffset << ", " << m_yOffset << ")";
			return ss.str();
		}

		float getXOffset() const { return m_xOffset; }
		float getYOffset() const { return m_yOffset; }

	private:
		float m_xOffset, m_yOffset;
	};

}