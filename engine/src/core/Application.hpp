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
    protected:
        virtual void Init();
        virtual void Shutdown();

        void InitImGui();
        void ShutdownImGui();

        virtual void OnEvent(Event& event);
        virtual void OnUpdate(const float dt);
        virtual void OnRender();
        
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        bool running_{false};

        std::unique_ptr<Scene> scene_;
        std::unique_ptr<Window> window_;

        uint32_t vertex_array_, vertex_buffer_, index_buffer_;
    };
}