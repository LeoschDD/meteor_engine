#pragma once

#include "scene/Scene.hpp"

namespace meteor
{
    class SceneSerializer
    {
    private:
        nlohmann::ordered_json SerializeEntity(ecs::Entity entity, ecs::World& world);
        void DeserializeEntity(nlohmann::ordered_json& json, ecs::World& world);

    public:
        SceneSerializer() = default;

        void Serialize(const std::filesystem::path& path, Scene& scene);
        std::unique_ptr<Scene> Deserialize(const std::filesystem::path& path);
    };
}