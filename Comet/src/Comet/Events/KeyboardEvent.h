#pragma once
#include "CometPCH.h"

#include "Event.h"
#include "Comet/Core/Input.h"

namespace Comet
{

	class KeyEvent : public Event
	{
	public:
		CMT_EVENT_CLASS_CATEGORY(EventCategory::EVENT_CATEGORY_KEYBOARD | EventCategory::EVENT_CATEGORY_INPUT)
		
		KeyCode getKeyCode() const { return m_keyCode; }

	protected:
		KeyEvent(KeyCode keyCode) : m_keyCode(keyCode) {}

	protected:
		KeyCode m_keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keyCode, int repeatCount = 0) : KeyEvent(keyCode), m_repeatCount(repeatCount) {}

		CMT_EVENT_CLASS_TYPE(KeyPressedEvent)
		
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}
		
		int getRepeatCount() const { return m_repeatCount; }

	private:
		int m_repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keyCode) : KeyEvent(keyCode) {}

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
		KeyTypedEvent(KeyCode keyCode) : KeyEvent(keyCode) {}

		CMT_EVENT_CLASS_TYPE(KeyTypedEvent)

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_keyCode;
			return ss.str();
		}
	};

}