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
        DrawMenu();
        DrawSceneSetting();
    }

    void DrawMenu()
    {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("New Scene"))
        {
            static char* buffer = new char[50];
            ImGui::InputText("Name", buffer, 50);
            if (ImGui::Button("Create")) GetSceneManager()->SetScene(std::make_unique<meteor::Scene>(std::string(buffer)));
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Load Scene"))
        {
            static char* buffer = new char[50];
            ImGui::InputText("Name", buffer, 50);
            if (ImGui::Button("Load")) GetSceneManager()->LoadScene(std::string(SCENES_DIR "/" + std::string(buffer) + ".msc"));
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Save Scene"))
        {
            GetSceneManager()->SaveScene(std::string(SCENES_DIR "/" + GetSceneManager()->GetScene()->GetName() + ".msc"));
        }
        ImGui::EndMainMenuBar();
    }

    void DrawSceneSetting()
    {
        ImGui::Begin("Scene Settings");
        auto scene = GetSceneManager()->GetScene();
        
        if (ImGui::Button("Create Entity"))
        {
            scene->GetWorld().Create();
        }

        for (meteor::ecs::Entity entity : scene->GetWorld())
        {
            if (ImGui::TreeNode(std::to_string(entity).c_str()))
            {
                if (ImGui::BeginMenu("Entity"))
                {
                    if (ImGui::Button("Add Parent"))
                    {
                        scene->GetWorld().AddComponent<meteor::ParentComponent>(entity, 0);
                    }  
                    if (ImGui::Button("Add Child"))
                    {
                        if (auto* children = scene->GetWorld().TryGetComponent<meteor::ChildrenComponent>(entity))
                        {
                            children->children.push_back(scene->GetWorld().Create());
                        }
                        else
                        {
                            scene->GetWorld().AddComponent<meteor::ChildrenComponent>(entity);
                            if (auto* children = scene->GetWorld().TryGetComponent<meteor::ChildrenComponent>(entity))
                            {
                                children->children.push_back(scene->GetWorld().Create());
                            }
                        }
                    }     
                    ImGui::EndMenu();                
                }
                ImGui::TreePop();
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