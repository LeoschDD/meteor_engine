#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"
#include "scene/Scene.hpp"
#include "core/Core.hpp"
#include "core/Window.hpp"

namespace meteor
{
    class Application
    {
    private:
        void Init();
        void Shutdown();
        
        void OnEvent(Event& event);

    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        bool running_{false};

        std::unique_ptr<Scene> scene_;
        std::unique_ptr<Window> window_;
    };
}