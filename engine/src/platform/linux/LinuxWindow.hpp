#pragma once

#include "core/Window.hpp"
#include "core/Log.hpp"
#include "core/Core.hpp"

namespace meteor
{
    class LinuxWindow : public Window
    {
    private:
        void Init();
        void Shutdown();

    public:
        LinuxWindow(uint32_t width, uint32_t height, const std::string& title);
        virtual ~LinuxWindow();

        virtual void OnUpdate() override;

        [[nodiscard]] virtual uint32_t GetWidth() const noexcept override {return width_;}
        [[nodiscard]] virtual uint32_t GetHeight() const noexcept override {return height_;}

        virtual void SetVSync(bool enabled) override;
        [[nodiscard]] virtual bool GetVSync() const noexcept override {return vsync_;}        

        [[nodiscard]] std::vector<std::unique_ptr<Event>>& GetEvents() noexcept override {return events_;}

    private:
        GLFWwindow* window_;

        uint32_t width_;
        uint32_t height_;
        std::string title_;
        bool vsync_{false};

        std::vector<std::unique_ptr<Event>> events_;
    };
}