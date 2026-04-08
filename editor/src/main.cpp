#include <Sandbox.hpp>

class EditorApp : public meteor::Application
{
public:
    void Init() override
    {
        meteor::Application::Init();

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        
        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    }

    void OnRender() override
    {
        meteor::Application::OnRender();
    }

private:

};

int main()
{
    EditorApp app;
    app.Run();
}