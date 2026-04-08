#include <Sandbox.hpp>

class EditorApp : public meteor::Application
{
public:
    void OnRender() override
    {
        meteor::Application::OnRender();
        OnImGui();
    }

    void OnImGui()
    {
        StartImGui();

        ImGui::ShowDemoWindow();

        EndImGui();
    }

    void StartImGui()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void EndImGui()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

private:

};

int main()
{
    EditorApp app;
    app.Run();
}