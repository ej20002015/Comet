#pragma once
#include "CometPCH.h"

#include "Event.h"
#include "Comet/Core/MouseCodes.h"

namespace Comet
{

class MouseButtonEvent : public Event
{
public:
	MouseButtonEvent(const MouseButtonCode mouseCode) : m_mouseCode(mouseCode) {}

	CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_MOUSE_BUTTON | EventCategory::EVENT_CATEGORY_MOUSE| EventCategory::EVENT_CATEGORY_INPUT)

	MouseButtonCode getKeyCode() const { return m_mouseCode; }

protected:
	const MouseButtonCode m_mouseCode;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(const MouseButtonCode mouseCode, const uint32_t repeatCount = 0) : MouseButtonEvent(mouseCode), m_repeatCount(repeatCount) {}

	CMT_EVENT_CLASS_TYPE(MouseButtonPressedEvent)

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_mouseCode << " (" << m_repeatCount << " repeats)";
		return ss.str();
	}

	int getRepeatCount() const { return m_repeatCount; }

private:
	const uint32_t m_repeatCount;
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(const MouseButtonCode mouseCode) : MouseButtonEvent(mouseCode) {}

	CMT_EVENT_CLASS_TYPE(MouseButtonReleasedEvent)

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_mouseCode;
		return ss.str();
	}
};

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(const float mousePosX, const float mousePosY) : m_mousePosX(mousePosX), m_mousePosY(mousePosY) {}

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
	const float m_mousePosX, m_mousePosY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(const float xOffset, const float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

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
	const float m_xOffset, m_yOffset;
};

}