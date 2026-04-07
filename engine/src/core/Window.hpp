#pragma once

#include "core/Headers.hpp"
#include "core/Event.hpp"

namespace meteor
{
    inline bool glfw_initialized = false;

    class Window
    {
    public:
        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        [[nodiscard]] virtual uint32_t GetWidth() const noexcept = 0;
        [[nodiscard]] virtual uint32_t GetHeight() const noexcept = 0;

        virtual void SetVSync(bool enabled) = 0;
        [[nodiscard]] virtual bool GetVSync() const noexcept = 0;

        [[nodiscard]] virtual std::vector<std::unique_ptr<Event>>& GetEvents() noexcept = 0;
    };
}