#include "core/Application.hpp"

meteor::Application::Application()
{
    window_ = std::make_unique<LinuxWindow>(1920, 1080, "Engine");
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    METEOR_CORE_ASSERT(status, "Failed to initialize GLAD");
}

meteor::Application::~Application()
{
}

void meteor::Application::Run()
{
    running_ = true;
    while (running_)
    {
        glClearColor(1, 0, 1, 0.5);
        glClear(GL_COLOR_BUFFER_BIT);
        window_->OnUpdate();

        for (auto& event : window_->GetEvents())
        {
            EventDispatcher dispatcher(*event);
            dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent& e)
            {
                running_ = false;
                return true;
            });
            scene_.OnEvent(*event);
        }
        window_->GetEvents().clear();

        scene_.OnUpdate(1);
    }
}