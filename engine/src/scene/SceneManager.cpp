#include "scene/SceneManager.hpp"
#include "core/Core.hpp"

meteor::SceneManager::~SceneManager()
{
    SetScene(nullptr);
}

bool meteor::SceneManager::LoadScene(const std::filesystem::path &path)
{
    auto scene = scene_serializer_.Deserialize(path);
    if (!scene)
    {
        METEOR_CORE_TRACE("Failed to load scene");
        return false;
    }
    SetScene(std::move(scene));
    return true;
}

void meteor::SceneManager::SaveScene(const std::filesystem::path& path)
{
    if (!scene_)
    {
        METEOR_CORE_WARN("Can't save scene - no active scene");
        return;
    }
    scene_serializer_.Serialize(path, *scene_.get());
}

void meteor::SceneManager::SetScene(std::unique_ptr<Scene> scene)
{
    if (scene_) scene_->OnDeactivate();
    if (scene)
    {
        scene->OnActivate();
        scene->OnStart();
    }
    scene_ = std::move(scene);
}
