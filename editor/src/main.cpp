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
                static std::array<char, 50> buffer;
                ImGui::InputText("Name", buffer.data(), 50);
                if (ImGui::Button("Create")) GetSceneManager()->SetScene(std::make_unique<meteor::Scene>(std::string(buffer.data())));
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Load Scene"))
            {
                static std::array<char, 50> buffer;
                ImGui::InputText("Name", buffer.data(), 50);
                if (ImGui::Button("Load")) GetSceneManager()->LoadScene(std::string(SCENES_DIR "/" + std::string(buffer.data()) + ".msc"));
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
                if (ImGui::MenuItem("Transform") && !world.HasComponent<meteor::TransformComponent>(entity))
                {   
                    world.AddComponent<meteor::TransformComponent>(entity);
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
        auto* transform = world.TryGetComponent<meteor::TransformComponent>(selected_);
        auto* global_transform = world.TryGetComponent<meteor::GlobalTransformComponent>(selected_);
        if (transform && global_transform && ImGui::CollapsingHeader("Transform"))
        {
            ImGui::DragFloat3("Translation", &transform->translation.x);

            glm::vec3 euler = glm::degrees(glm::eulerAngles(transform->rotation));
            ImGui::DragFloat3("Rotation", &euler.x);
            transform->rotation = glm::quat(glm::radians(euler));

            ImGui::DragFloat3("Scale", &transform->scale.x);

            ImGui::Separator();

            ImGui::BeginDisabled();

            glm::vec3 global_translation(global_transform->transform[3]);
            glm::quat global_rotation = glm::quat_cast(global_transform->transform);
            glm::vec3 global_scale;
            global_scale.x = glm::length(glm::vec3(global_transform->transform[0]));
            global_scale.y = glm::length(glm::vec3(global_transform->transform[1]));
            global_scale.z = glm::length(glm::vec3(global_transform->transform[2]));

            ImGui::DragFloat3("Global Translation", &global_translation.x);

            glm::vec3 global_euler = glm::degrees(glm::eulerAngles(global_rotation));
            ImGui::DragFloat3("Global Rotation", &global_euler.x);

            ImGui::DragFloat3("Global Scale", &global_scale.x);

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