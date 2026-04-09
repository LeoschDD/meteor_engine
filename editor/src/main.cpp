#include <Sandbox.hpp>

class EditorApp : public meteor::Application
{
public:
    void OnRender() override
    {
        meteor::Application::OnRender();

        ImGuiBegin();
        OnImGui();
        ImGuiEnd();
    }

    void OnImGui()
    {
        ImGui::Begin("Settings");

        for (auto& [key, scene] : GetSceneManager()->GetScenes())
        {
            if (ImGui::Button("Create Entity"))
            {
                scene->CreateEntity();
            }

            for (meteor::ecs::Entity entity : scene->GetWorld())
            {
                if (auto uuid = scene->GetWorld().TryGetComponent<meteor::UUIDComponent>(entity)->uuid.GetUUID())
                {
                    if (ImGui::TreeNode(std::to_string(uuid).c_str()))
                    {
                        if (ImGui::Button("Add Component"))
                        {
                            scene->GetWorld().AddComponent<meteor::ParentComponent>(entity, 131313);
                        }
                        ImGui::TreePop();
                    }
                    
                }
            }
        }

        ImGui::End();
    }

    void ImGuiBegin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiEnd()
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