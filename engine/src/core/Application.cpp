#include "core/Application.hpp"

void meteor::Application::Init()
{
    window_ = std::make_unique<Window>(1920, 1080, "Meteor Engine");

    int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    METEOR_CORE_ASSERT(success, "Failed to initialize glad");

    InitImGui();

    scene_manager_ = std::make_unique<SceneManager>();
    if (!scene_manager_->LoadScene(std::string(SCENES_DIR "/start_scene.msc")))
    {
        scene_manager_->SetScene(std::make_unique<Scene>("start_scene"));
    }

    shader_ = std::make_unique<Shader>(ENGINE_DIR "/resources/shaders/basic.vs", ENGINE_DIR "/resources/shaders/basic.fs");
}

void meteor::Application::Shutdown()
{
    scene_manager_->SetScene(nullptr);
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

    scene_manager_->GetScene()->OnEvent(event);
}

void meteor::Application::OnUpdate(const float dt)
{
    scene_manager_->GetScene()->OnUpdate(dt);
}

void meteor::Application::OnRender()
{
    shader_->Bind();
    scene_manager_->GetScene()->OnRender();
    glBindVertexArray(vertex_array_);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    shader_->Unbind();
}

meteor::Application::Application()
{
    Init();

    glGenVertexArrays(1, &vertex_array_);
    glBindVertexArray(vertex_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

    float vertices[9]  = {
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f
    };
        
    uint32_t indices[3] = {
        0, 1, 2
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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

        glClearColor(0.15, 0.15, 0.15, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        OnRender();

        window_->OnUpdate();
    }
}

