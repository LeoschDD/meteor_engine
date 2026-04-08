#include "serialization/WorldSerializer.hpp"
#include "components/ChildrenComponent.hpp"
#include "components/ParentComponent.hpp"
#include "components/UUIDComponent.hpp"

nlohmann::ordered_json meteor::SceneSerializer::SerializeEntity(ecs::Entity entity, ecs::World& world)
{
    nlohmann::ordered_json json;

    if (auto* uuid = world.TryGetComponent<UUIDComponent>(entity))
    {
        json["uuid"] = uuid->uuid_;
    } 
    if (auto* parent = world.TryGetComponent<ParentComponent>(entity))
    {
        json["parent"] = parent->parent_;
    }
    if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
    {
        json["children"] = children->children_;
        
    }   
    return json;
}

void meteor::SceneSerializer::DeserializeEntity(nlohmann::ordered_json& json, ecs::World& world)
{
    auto entity = world.Create();

    if (json.contains("uuid"))
    {
        world.AddComponent<UUIDComponent>(entity, json["uuid"].get<uint64_t>());
    }
    if (json.contains("parent"))
    {
        world.AddComponent<ParentComponent>(entity, json["parent"].get<uint64_t>());
    }
    if (json.contains("children"))
    {
        world.AddComponent<ChildrenComponent>(entity, json["children"].get<std::vector<uint64_t>>());
    }
}

void meteor::SceneSerializer::Serialize(const std::filesystem::path& path, Scene& scene)
{
    nlohmann::ordered_json json;
    ecs::World& world = scene.GetWorld();

    json["name"] = scene.GetName();
    for (auto& entity : world)
    {
        json["world"].push_back(SerializeEntity(entity, world));
    }

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
    std::ifstream file(path);
    if (!file.is_open())
    {
        METEOR_CORE_ERROR("Could not open file: {}", path.string());
        return nullptr;
    }

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
    if (!json.contains("name") || !json.contains("world"))
    {
        METEOR_CORE_ERROR("Scene file invalid: {}", path.string());
        return nullptr; 
    }
    std::string name = json["name"].get<std::string>();

    ecs::World world;
    world.RegisterComponent<UUIDComponent>();
    world.RegisterComponent<ParentComponent>();
    world.RegisterComponent<ChildrenComponent>();

    for (auto& entity : json["world"])
    {
        DeserializeEntity(entity, world);
    }
    return std::make_unique<Scene>(name, std::move(world));
}