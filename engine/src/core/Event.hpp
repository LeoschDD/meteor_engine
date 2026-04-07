#pragma once

#include "core/Headers.hpp"

namespace meteor
{
    class EventDispatcher;

    ////////////////////////////////////////////////////
    // Event base class ////////////////////////////////
    ////////////////////////////////////////////////////

    class Event
    {
    private:
        friend class EventDispatcher;

    protected:
        Event() = default;

    public:
        virtual ~Event() = default;

    private:
        bool handled_;
    };

    ////////////////////////////////////////////////////
    // Key events //////////////////////////////////////
    ////////////////////////////////////////////////////

    class KeyEvent : public Event
    {
    protected:
        KeyEvent(const uint32_t key_code)
            : key_code_(key_code)
        {}
    
    public:
        uint32_t GetKeyCode() const noexcept
        {
            return key_code_;
        }
    
    private:
        const uint32_t key_code_;
    };
    
    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(const uint32_t key_code, bool repeat)
            : KeyEvent(key_code)
            , repeat_(repeat)
        {}

    private:
        bool repeat_;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(const uint32_t key_code)
            : KeyEvent(key_code)
        {}
    };

    ////////////////////////////////////////////////////
    // App events //////////////////////////////////////
    ////////////////////////////////////////////////////

    class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: width_(width), height_(height) {}

		[[nodiscard]] unsigned int GetWidth() const noexcept {return width_;}
		[[nodiscard]] unsigned int GetHeight() const noexcept {return height_;}

	private:
		unsigned int width_, height_;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;
	};

    ////////////////////////////////////////////////////
    // Mouse events ////////////////////////////////////
    ////////////////////////////////////////////////////

    class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: mouse_x_(x), mouse_y(y) {}

		float GetX() const {return mouse_x_;}
		float GetY() const {return mouse_y;}

	private:
		float mouse_x_, mouse_y;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float x, const float y)
			: offset_x_(x), offset_y_(y) {}

		[[nodiscard]] float GetOffsetX() const noexcept { return offset_x_; }
		[[nodiscard]] float GetOffsetY() const noexcept { return offset_y_; }

	private:
		float offset_x_, offset_y_;
	};

	class MouseButtonEvent : public Event
	{
	public:
		uint32_t GetMouseButton() const { return button_; }

	protected:
		MouseButtonEvent(const uint32_t button)
			: button_(button) {}

    private:
		uint32_t button_;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const uint32_t button)
			: MouseButtonEvent(button) {}
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const uint32_t button)
			: MouseButtonEvent(button) {}
	};

    ////////////////////////////////////////////////////
    // Event dispatcher ////////////////////////////////
    ////////////////////////////////////////////////////

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : event_(event)
        {}

        template<typename T, typename Fn>
        bool Dispatch(Fn&& fn)
        {
            if (T* e = dynamic_cast<T*>(&event_))
            {
                if (fn(*e)) event_.handled_ = true;
                return true;
            }
            return false;
        }

    private:
        Event& event_;
    };
}