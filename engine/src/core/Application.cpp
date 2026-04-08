#include "core/Application.hpp"

void meteor::Application::Init()
{
    scene_ = std::make_unique<Scene>();
    scene_->OnStart();
    scene_->OnActivate();
    window_ = std::make_unique<Window>(1920, 1080, "Engie");

    int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    METEOR_CORE_ASSERT(success, "Failed to initialize glad");

    InitImGui();
}

void meteor::Application::Shutdown()
{
    scene_->OnDeactivate();
    ShutdownImGui();
}

void meteor::Application::InitImGui()
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 

    ImGui_ImplGlfw_InitForOpenGL(window_->GetNative(), true);
    ImGui_ImplOpenGL3_Init();
}

void meteor::Application::ShutdownImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

    scene_->OnEvent(event);
}

void meteor::Application::OnUpdate(const float dt)
{
    scene_->OnUpdate(dt);
}

void meteor::Application::OnRender()
{
    scene_->OnRender();
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
        window_->OnUpdate();

        for (auto& event : window_->GetEventQueue())
        {
            OnEvent(*event);
        }
        window_->ClearEventQueue();

        OnUpdate(0.1);

        glClearColor(1, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        OnRender();

        window_->OnUpdate();
    }
}

