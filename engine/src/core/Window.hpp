#pragma once

#include "core/Headers.hpp"
#include "core/Event.hpp"
#include "core/Core.hpp"
#include "core/Log.hpp"

namespace meteor
{
    class Window
    {
    private:
        using EventQueue = std::vector<std::unique_ptr<Event>>;
    private:
        void Init();
        void Shutdown();
        void SetEventCallbacks();

    public:
        Window(int width, int height, const std::string& title);
        ~Window();
        
        void OnUpdate();

        void SetVSync(bool enabled);
        [[nodiscard]] bool GetVSync() const noexcept {return vsync_;}

        [[nodiscard]] uint32_t GetWidth() const noexcept {return width_;}
        [[nodiscard]] uint32_t GetHeight() const noexcept {return height_;}

        void ClearEventQueue() noexcept {event_queue_.clear();}
        [[nodiscard]] const EventQueue& GetEventQueue() const noexcept {return event_queue_;}

        [[nodiscard]] GLFWwindow* GetNative() noexcept {return window_;}

    private:
        GLFWwindow* window_;
        EventQueue event_queue_;
        bool vsync_;
        uint32_t width_, height_;
        std::string title_;
    };
}