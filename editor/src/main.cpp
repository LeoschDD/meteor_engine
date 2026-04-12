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
        DrawSceneTree();
        DrawInspector();
    }

    ////////////////////////////////////////////////////
    // Menu bar ////////////////////////////////////////
    ////////////////////////////////////////////////////

    void DrawMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
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
    }

    ////////////////////////////////////////////////////
    // Scene tree window ///////////////////////////////
    ////////////////////////////////////////////////////

    void DrawSceneTree()
    {
        auto& world =  GetSceneManager()->GetScene()->GetWorld();
        ImGui::Begin("Scene Tree");
        if (ImGui::BeginPopupContextWindow("Scene Tree Popup", ImGuiPopupFlags_NoOpenOverItems))
        {
            if (ImGui::MenuItem("Create Entity"))
            {
                world.Create();
            }
            ImGui::EndPopup();
        }
        for (auto entity : world)
        {
            // Begin recursive tree on all root entities without parent
            if (!world.HasComponent<meteor::ParentComponent>(entity))
            {
                DrawNodesRecursive(entity, world);
            }
        }

        ImGui::End();
    }

    void DrawNodesRecursive(meteor::ecs::Entity entity, meteor::ecs::World& world)
    {
        // Set flags for node
        ImGuiTreeNodeFlags flags = 0;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesToNodes | ImGuiTreeNodeFlags_OpenOnArrow;
        if (selected_ == entity) flags |= ImGuiTreeNodeFlags_Selected;
        if (!world.HasComponent<meteor::ChildrenComponent>(entity)) flags |= ImGuiTreeNodeFlags_Leaf;

        // Begin tree node
        bool open = ImGui::TreeNodeEx(std::format("Entity {}", std::to_string(entity)).c_str(), flags);
        
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen())
        {
            selected_ = entity;
        }

        // If mouseclick right begin popup
        if (ImGui::BeginPopupContextItem(nullptr))
        {
            if (ImGui::MenuItem("Add Child"))
            {
                auto e = world.Create();
                world.AddComponent<meteor::ParentComponent>(e, entity);
                if (auto* children = world.TryGetComponent<meteor::ChildrenComponent>(entity)) 
                {
                    children->children.push_back(e);
                }
                else
                {
                    world.AddComponent<meteor::ChildrenComponent>(entity);
                    if (auto* children = world.TryGetComponent<meteor::ChildrenComponent>(entity)) 
                    {
                        children->children.push_back(e);
                    }
                }
            }
            if (ImGui::BeginMenu("Add Component"))
            {
                if (ImGui::MenuItem("Transform 3D") && !world.HasComponent<meteor::Transform3DComponent>(entity))
                {   
                    world.AddComponent<meteor::Transform3DComponent>(entity);
                    world.AddComponent<meteor::GlobalTransform3DComponent>(entity);
                }
                if (ImGui::MenuItem("Transform 2D") && !world.HasComponent<meteor::Transform2DComponent>(entity))
                {   
                    world.AddComponent<meteor::Transform2DComponent>(entity);
                    world.AddComponent<meteor::GlobalTransform2DComponent>(entity);
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }

        // Draw children recursive if node opened
        if (open)
        {
            if (auto* children = world.TryGetComponent<meteor::ChildrenComponent>(entity))
            {
                for (auto child : children->children)
                {
                    DrawNodesRecursive(child, world);
                }
            } 
            ImGui::TreePop(); 
        }
    }

    ////////////////////////////////////////////////////
    // Entity Inspector ////////////////////////////////
    ////////////////////////////////////////////////////

    void DrawInspector()
    {
        auto& world =  GetSceneManager()->GetScene()->GetWorld();
        ImGui::Begin("Inspector");
        if (selected_ == meteor::ecs::INVALID_ENTITY) 
        {
            ImGui::End();
            return;
        }
        auto* transform_3d = world.TryGetComponent<meteor::Transform3DComponent>(selected_);
        auto* global_transform_3d = world.TryGetComponent<meteor::GlobalTransform3DComponent>(selected_);
        if (transform_3d && global_transform_3d && ImGui::CollapsingHeader("Transform 3D"))
        {
            ImGui::DragFloat3("Position", &transform_3d->translation.x);

            glm::vec3 euler = glm::degrees(glm::eulerAngles(transform_3d->rotation));
            ImGui::DragFloat3("Rotation", &euler.x);
            transform_3d->rotation = glm::quat(glm::radians(euler));

            ImGui::DragFloat3("Scale", &transform_3d->scale.x);

            ImGui::BeginDisabled();

            ImGui::DragFloat3("Global Position", &global_transform_3d->translation.x);

            glm::vec3 global_euler = glm::degrees(glm::eulerAngles(global_transform_3d->rotation));
            ImGui::DragFloat3("Global Rotation", &global_euler.x);
            global_transform_3d->rotation = glm::quat(glm::radians(global_euler));

            ImGui::DragFloat3("Global Scale", &global_transform_3d->scale.x);

            ImGui::EndDisabled();
                      
        }
        auto* transform_2d = world.TryGetComponent<meteor::Transform2DComponent>(selected_);
        auto* global_transform_2d = world.TryGetComponent<meteor::GlobalTransform2DComponent>(selected_);
        if (transform_2d && global_transform_2d && ImGui::CollapsingHeader("Transform 2D"))
        {
            ImGui::DragFloat2("Position", &transform_2d->translation.x);
            ImGui::DragFloat("Rotation", &transform_2d->rotation);
            ImGui::DragFloat2("Scale", &transform_2d->scale.x);

            ImGui::BeginDisabled();

            ImGui::DragFloat2("Global Position", &global_transform_2d->translation.x);
            ImGui::DragFloat("Global Rotation", &transform_2d->rotation);
            ImGui::DragFloat2("Global Scale", &global_transform_2d->scale.x);

            ImGui::EndDisabled();
                      
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
    meteor::ecs::Entity selected_{meteor::ecs::INVALID_ENTITY};
};

int main()
{
    EditorApp app;
    app.Run();
}