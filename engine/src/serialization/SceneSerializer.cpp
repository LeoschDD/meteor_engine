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
    if (auto* transform = world.TryGetComponent<Transform3DComponent>(entity))
    {
        glm::vec3 rotation = glm::eulerAngles(transform->rotation);

        json["transform3D"]["position"] = {transform->position.x, transform->position.y, transform->position.z};
        json["transform3D"]["scale"] = {transform->scale.x, transform->scale.y, transform->scale.z};
        json["transform3D"]["rotation"] = {rotation.x,rotation.y, rotation.z};
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
    if (json.contains("transform3D"))
    {
        Transform3DComponent transform;
        if (json["transform3D"].contains("position"))
        {
            auto position = json["transform3D"]["position"].get<std::array<float, 3>>();
            transform.position = glm::vec3(position[0], position[1], position[2]);
        }
        if (json["transform3D"].contains("scale"))
        {
            auto scale = json["transform3D"]["scale"].get<std::array<float, 3>>();
            transform.scale = glm::vec3(scale[0], scale[1], scale[2]);
        }
        if (json["transform3D"].contains("rotation"))
        {
            auto rotation = json["transform3D"]["rotation"].get<std::array<float, 3>>();
            transform.rotation = glm::quat(glm::vec3(rotation[0], rotation[1], rotation[2]));
        }
        world.AddComponent<Transform3DComponent>(entity, transform);
        world.AddComponent<GlobalTransform3DComponent>(entity);
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