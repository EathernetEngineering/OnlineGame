#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <Engine/KeyCodes.h>

namespace cee
{
	namespace engine
	{
		enum class EventType
		{
			None = 0,
			WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
			AppTick, AppUpdate, AppRender,
			KeyPressed, KeyReleased, KeyTyped,
			MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
		};
		
		enum EventCategory
		{
			EventCategoryNone         = 0,
			EventCategoryApplication  = 1 << 0,
			EventCategoryInput        = 1 << 1,
			EventCategoryKeyboard     = 1 << 2,
			EventCategoryMouse        = 1 << 3,
			EventCategoryMouseButton  = 1 << 4,
		};
		
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::type; }\
		virtual EventType GetEventType() const override { return GetStaticType(); }\
		virtual const char* GetName() const override { return #type;}
		
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
		
		class Event
		{
		public:
			virtual ~Event() = default;
			
			bool handled = false;
			
			virtual EventType GetEventType() const = 0;
			virtual const char* GetName() const  = 0;
			virtual int GetCategoryFlags() const = 0;
			virtual std::string ToString() const { return GetName(); }
			
			bool IsInCategory(EventCategory category) { return category & GetCategoryFlags(); }
		};
		
		class WindowResizeEvent : public Event
		{
		public:
			WindowResizeEvent(uint32_t width, uint32_t height)
				: m_Width(width), m_Height(height)
			{
			}
				
			uint32_t GetWidth() const { return m_Width; }
			uint32_t GetHeight() const { return m_Height; }
			
			std::string ToString() const override
			{
				char str[128];
				sprintf(str, "WindowResizeEvent: (%u, %u)", m_Width, m_Height);
				return str;
			}
			
			EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
			
		private:
			uint32_t m_Width, m_Height;
		};
		
		class WindowCloseEvent : public Event
		{
		public:
			WindowCloseEvent() = default;
			
			EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
		};
		
		class AppTickEvent : public Event
		{
		public:
			AppTickEvent() = default;
			
			EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
		};
		class AppUpdateEvent : public Event
		{
		public:
			AppUpdateEvent() = default;
			
			EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
		};
		
		class AppRenderEvent : public Event
		{
		public:
			AppRenderEvent() = default;
			
			EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
		};
		
		class KeyEvent : public Event
		{
		protected:
			KeyEvent(const KeyCode keycode)
				: m_KeyCode(keycode)
			{
			}
			
		public:
			KeyCode GetKeyCode() const { return m_KeyCode; }
			
			EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
			
		protected:
			KeyCode m_KeyCode;
		};
		
		class KeyPressedEvent : public KeyEvent
		{
		public:
			KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount)
				: KeyEvent(keycode), m_RepeatCount(repeatCount)
			{
			}
			
			uint16_t GetRepeatCount() const { return m_RepeatCount; }
			
			std::string ToSting()
			{
				char str[128];
				sprintf(str, "KeyPressedEvent: %#2hX (%hu repeats)", m_KeyCode, m_RepeatCount);
				return str;
			}
			
			EVENT_CLASS_TYPE(KeyPressed)
			
		private:
			uint16_t m_RepeatCount;
		};
		
		class KeyReleasedEvent : public KeyEvent
		{
		public:
			KeyReleasedEvent(const KeyCode keycode)
				: KeyEvent(keycode)
			{
			}
			
			std::string ToSting()
			{
				char str[128];
				sprintf(str, "KeyReleasedEvent: %#2hX", m_KeyCode);
				return str;
			}
			
			EVENT_CLASS_TYPE(KeyReleased)
		};
		
		class KeyTypedEvent : public KeyEvent
		{
		public:
			KeyTypedEvent(const KeyCode keycode)
				: KeyEvent(keycode)
			{
			}
			
			std::string ToSting()
			{
				char str[128];
				sprintf(str, "KeyTypedEvent: %#2hX", m_KeyCode);
				return str;
			}
			
			EVENT_CLASS_TYPE(KeyTyped)
		};
		
		class MouseMovedEvent : public Event
		{
		public:
			MouseMovedEvent(const float x, const float y)
				: m_MouseX(x), m_MouseY(y)
			{
			}
				
			uint32_t GetX() const { return m_MouseX; }
			uint32_t GetY() const { return m_MouseY; }
			
			std::string ToString() const override
			{
				char str[128];
				sprintf(str, "MouseMovedEvent: (%f, %f)", m_MouseX, m_MouseY);
				return str;
			}
			
			EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			
		private:
			float m_MouseX, m_MouseY;
		};
		
		class MouseScrolledEvent : public Event
		{
		public:
			MouseScrolledEvent(const float xOffset, const float yOffset)
				: m_XOffset(xOffset), m_YOffset(yOffset)
			{
			}
				
			uint32_t GetXOffset() const { return m_XOffset; }
			uint32_t GetYOffset() const { return m_YOffset; }
			
			std::string ToString() const override
			{
				char str[128];
				sprintf(str, "MouseScrolledEvent: (%f, %f)", m_XOffset, m_YOffset);
				return str;
			}
			
			EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			
		private:
			float m_XOffset, m_YOffset;
		};
		
		class MouseButtonEvent : public Event
		{
		protected:
			MouseButtonEvent(const MouseCode button)
				: m_Button(button)
			{
			}
			
		public:
			KeyCode GetMouseButtom() const { return m_Button; }
			
			EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)
			
		protected:
			KeyCode m_Button;
		};
		
		class MouseButtonPressedEvent : public MouseButtonEvent
		{
		public:
			MouseButtonPressedEvent(const MouseCode button)
				: MouseButtonEvent(button)
			{
			}
			
			std::string ToSting()
			{
				char str[128];
				sprintf(str, "MouseButtonPressedEvent: %#2hX", m_Button);
				return str;
			}
			
			EVENT_CLASS_TYPE(MouseButtonPressed)
		};
		
		class MouseButtonReleasedEvent : public MouseButtonEvent
		{
		public:
			MouseButtonReleasedEvent(const MouseCode button)
				: MouseButtonEvent(button)
			{
			}
			
			std::string ToSting()
			{
				char str[128];
				sprintf(str, "MouseButtonReleasedEvent: %#2hX", m_Button);
				return str;
			}
			
			EVENT_CLASS_TYPE(MouseButtonReleased)
		};
		
		class EventDispatcher
		{
		public:
			EventDispatcher(Event& event)
				: m_Event(event)
			{
			}
			
			template<typename T, typename F>
			bool Dispatch(const F& func)
			{
				if (m_Event.GetEventType() == T::GetStaticType())
				{
					m_Event.handled |= func(static_cast<T&>(m_Event));
					return true;
				}
				return false;
			}
			
		private:
			Event& m_Event;
		};
	}
}
