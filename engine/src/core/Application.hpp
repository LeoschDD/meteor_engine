#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"
#include "scene/SceneManager.hpp"
#include "core/Core.hpp"
#include "core/Window.hpp"
#include "renderer/Shader.hpp"
#include "renderer/Buffer.hpp"
#include "renderer/VertexArray.hpp"

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

        SceneManager* GetSceneManager() {return scene_manager_.get();}
        Window* GetWindow() {return window_.get();}

    private:
        bool running_{false};

        std::unique_ptr<SceneManager> scene_manager_;
        std::unique_ptr<Window> window_;

        std::unique_ptr<VertexBuffer> vertex_buffer_;
        std::unique_ptr<IndexBuffer> index_buffer_;
        std::unique_ptr<VertexArray> vertex_array_;

        std::unique_ptr<Shader> shader_;
    };
}