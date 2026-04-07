#include "core/Application.hpp"
#include "Application.hpp"

void meteor::Application::Init()
{
    scene_ = std::make_unique<Scene>();
    window_ = std::make_unique<Window>(1920, 1080, "Engie");

    int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    METEOR_CORE_ASSERT(success, "Failed to initialize glad");
}

void meteor::Application::Shutdown()
{
}

void meteor::Application::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent& event)
    {
        running_ = false;
        return true;
    });
    dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& event)
    {
        if (event.GetKeyCode() == GLFW_KEY_ESCAPE) 
        {
            running_ = false; 
            return true;
        }
        return false;
    });
}

meteor::Application::Application()
{
    Init();
}

meteor::Application::~Application()
{
    Shutdown();
}

void meteor::Application::Run()
{
    running_ = true;
    while (running_)
    {
        glClearColor(1, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        window_->OnUpdate();

        for (auto& event : window_->GetEventQueue())
        {
            OnEvent(*event);
            scene_->OnEvent(*event);
        }
        window_->ClearEventQueue();

        scene_->OnUpdate(0.1);
    }
}