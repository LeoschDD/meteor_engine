#pragma once

#include "scene/Scene.hpp"
#include "serialization/SceneSerializer.hpp"

namespace meteor
{
    class SceneManager
    {
    public:
        SceneManager() = default;
        ~SceneManager();

        bool LoadScene(const std::filesystem::path& path);
        void SaveScene(const std::filesystem::path& path);
        void SetScene(std::unique_ptr<Scene> scene);
        [[nodiscard]] Scene* GetScene() noexcept {return scene_.get();}

    private:
        std::unique_ptr<Scene> scene_;
        SceneSerializer scene_serializer_;
    };
}