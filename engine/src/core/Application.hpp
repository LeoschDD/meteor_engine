#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"
#include "scene/Scene.hpp"
#include "core/Core.hpp"
#include "core/Window.hpp"
#include "platform/linux/LinuxWindow.hpp"

namespace meteor
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        bool running_{false};
        std::unique_ptr<Window> window_;
        Scene scene_;
    };
}