#include "serialization/SceneSerializer.hpp"
#include "components/BasicComponents.hpp"
#include "components/TransformComponents.hpp"

nlohmann::ordered_json meteor::SceneSerializer::SerializeEntity(ecs::Entity entity, ecs::World& world, std::unordered_map<meteor::ecs::Entity, uint64_t>& map)
{
    nlohmann::ordered_json json;
    if (!map.contains(entity)) return json;

    json["id"] = map[entity];
    if (auto* parent = world.TryGetComponent<ParentComponent>(entity))
    {
         if (map.contains(parent->parent)) json["parent"] = map[parent->parent];
    }
    if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
    {
        for (auto child : children->children)
        {
            if (map.contains(child)) json["children"].push_back(map[child]); 
        }
    }   
    if (const auto* transform = static_cast<const Transform3DComponent*>(world.TryGetComponent<Transform3DComponent>(entity)))
    {
        glm::vec3 rotation = glm::eulerAngles(transform->Rotation());
        json["transform3d"]["translation"] = {transform->Translation().x, transform->Translation().y, transform->Translation().z};
        json["transform3d"]["rotation"] = {rotation.x,rotation.y, rotation.z};
        json["transform3d"]["scale"] = {transform->Scale().x, transform->Scale().y, transform->Scale().z};
    }
    if (const auto* transform = static_cast<const Transform2DComponent*>(world.TryGetComponent<Transform2DComponent>(entity)))
    {
        json["transform2d"]["translation"] = {transform->Translation().x, transform->Translation().y};
        json["transform2d"]["rotation"] = transform->Rotation();
        json["transform2d"]["scale"] = {transform->Scale().x, transform->Scale().y};
    }
    return json;
}

void meteor::SceneSerializer::DeserializeEntity(nlohmann::ordered_json& json, ecs::World& world, std::unordered_map<uint64_t, meteor::ecs::Entity>& map)
{
    if (!json.contains("id")) return;
    if (!map.contains(json["id"])) return;

    auto entity = map[json["id"]];

    if (json.contains("parent"))
    {
        auto file_id = json["parent"].get<uint64_t>();
        if (map.contains(file_id)) world.AddComponent<ParentComponent>(entity, map[file_id]);
    }
    if (json.contains("children"))
    {
        std::vector<ecs::Entity> temp_children;
        for (auto child : json["children"])
        {
            auto file_id = child.get<uint64_t>();
            if (map.contains(file_id)) temp_children.push_back(map[file_id]);
        }
        if (!temp_children.empty()) world.AddComponent<ChildrenComponent>(entity, temp_children);
    }
    if (json.contains("transform3d"))
    {
        world.AddComponent<Transform3DComponent>(entity);
        if (auto* transform = world.TryGetComponent<Transform3DComponent>(entity))
        {
            if (json["transform3d"].contains("translation"))
            {
                auto translation = json["transform3d"]["translation"].get<std::array<float, 3>>();
                transform->Translation() = glm::vec3(translation[0], translation[1], translation[2]);
            }
            if (json["transform3d"].contains("rotation"))
            {
                auto rotation = json["transform3d"]["rotation"].get<std::array<float, 3>>();
                transform->Rotation() = glm::quat(glm::vec3(rotation[0], rotation[1], rotation[2]));
            }
            if (json["transform3d"].contains("scale"))
            {
                auto scale = json["transform3d"]["scale"].get<std::array<float, 3>>();
                transform->Scale() = glm::vec3(scale[0], scale[1], scale[2]);
            }
        }
    }
    if (json.contains("transform2d"))
    {
        world.AddComponent<Transform2DComponent>(entity);
        if (auto* transform = world.TryGetComponent<Transform2DComponent>(entity))
        {
            if (json["transform2d"].contains("translation"))
            {
                auto translation = json["transform2d"]["translation"].get<std::array<float, 2>>();
                transform->Translation() = glm::vec2(translation[0], translation[1]);
            }
            if (json["transform2d"].contains("rotation"))
            {
                auto rotation = json["transform2d"]["rotation"].get<float>();
                transform->Rotation() = rotation;
            }
            if (json["transform2d"].contains("scale"))
            {
                auto scale = json["transform2d"]["scale"].get<std::array<float, 2>>();
                transform->Scale() = glm::vec2(scale[0], scale[1]);
            }
        }
    }
}

void meteor::SceneSerializer::Serialize(const std::filesystem::path& path, Scene& scene)
{
    // Create json
    nlohmann::ordered_json json;
    json["name"] = scene.GetName();

    // File id gets created for every entity 
    std::unordered_map<ecs::Entity, uint64_t> map;
    uint64_t file_id = 0;
    for (auto& entity : scene.GetWorld())
    {
        map[entity] = file_id++;
    }
    // Emplace every entity as list of file id and components to json 
    for (auto& entity : scene.GetWorld())
    {
        json["world"].push_back(SerializeEntity(entity, scene.GetWorld(), map));
    }

    // Write json to file
    std::ofstream file(path);
    if (!file.is_open())
    {
        METEOR_CORE_ERROR("Could not open file: {}", path.string());
        return;
    }
    file << json.dump(4);
    file.close();
}

std::unique_ptr<meteor::Scene> meteor::SceneSerializer::Deserialize(const std::filesystem::path& path)
{
    // Check if file can be opened
    std::ifstream file(path);
    if (!file.is_open())
    {
        METEOR_CORE_WARN("Could not open file: {}", path.string());
        return nullptr;
    }

    // Check if scene file can be parsed and is valid
    nlohmann::ordered_json json;
    try
    {
        json = nlohmann::json::parse(file);
    } 
    catch (const nlohmann::json::parse_error& e)
    {
        METEOR_CORE_ERROR("Failed to parse scene file: {}", e.what());
        return nullptr;
    }
    if (!json.contains("name"))
    {
        METEOR_CORE_ERROR("Scene file invalid: {}", path.string());
        return nullptr; 
    }

    // Create world and scene from json
    std::string name = json["name"].get<std::string>();
    ecs::World world;
    if (json.contains("world"))
    {
        // Create entity for every json entry and map file id to the entity 
        std::unordered_map<uint64_t, ecs::Entity> map;
        for (auto& entity : json["world"])
        {
            if (!entity.contains("id")) continue;
            auto e = world.Create();
            map[entity["id"]] = e;
        }
        // Add components for every json entry and use map to get entities from file ids
        for (auto& entity : json["world"])
        {
            DeserializeEntity(entity, world, map);
        }        
    }
    return std::make_unique<Scene>(name, std::move(world));
}