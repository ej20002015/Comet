#pragma once
#include "CometPCH.h"

#include "Event.h"
#include "Comet/Core/KeyCodes.h"

namespace Comet
{

class KeyEvent : public Event
{
public:
	CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_KEYBOARD | EventCategory::EVENT_CATEGORY_INPUT)
		
	KeyCode getKeyCode() const { return m_keyCode; }

protected:
	KeyEvent(const KeyCode keyCode) : m_keyCode(keyCode) {}

protected:
	const KeyCode m_keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(const KeyCode keyCode, const bool repeat = false) : KeyEvent(keyCode), m_repeat(repeat) {}

	CMT_EVENT_CLASS_TYPE(KeyPressedEvent)
		
	std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_keyCode << " (repeat = " << m_repeat << ")";
		return ss.str();
	}
		
	bool getRepeat() const { return m_repeat; }

private:
	const bool m_repeat;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(const KeyCode keyCode) : KeyEvent(keyCode) {}

	CMT_EVENT_CLASS_TYPE(KeyReleasedEvent)

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_keyCode;
		return ss.str();
	}
};

//To support text input using unicode characters
class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(const KeyCode keyCode) : KeyEvent(keyCode) {}

	CMT_EVENT_CLASS_TYPE(KeyTypedEvent)

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_keyCode;
		return ss.str();
	}
};

}