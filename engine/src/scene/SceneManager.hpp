#pragma once

#include "scene/Scene.hpp"
#include "serialization/WorldSerializer.hpp"

namespace meteor
{
    class SceneManager
    {
    private:
        using SceneMap = std::unordered_map<std::string, std::unique_ptr<Scene>>;
    
    public:
        explicit SceneManager(const std::filesystem::path& scene_folder)
            : scene_folder_(scene_folder)
        {}

        ~SceneManager() = default;

        bool LoadScene(const std::string& name);
        void SaveScene(const std::string& name);
        void CreateScene(const std::string& name);

        void Clear();

        [[nodiscard]] Scene& GetScene(const std::string& name);
        [[nodiscard]] SceneMap& GetScenes() noexcept {return scenes_;}

    private:
        std::filesystem::path scene_folder_;
        SceneSerializer scene_serializer_;
        SceneMap scenes_;
    };
}