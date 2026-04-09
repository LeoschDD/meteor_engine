#include "scene/SceneManager.hpp"
#include "core/Core.hpp"

bool meteor::SceneManager::LoadScene(const std::string& name)
{
    if (scenes_.contains(name)) 
    {
        METEOR_CORE_WARN("Tried to load scene {} that is already loaded", name);
        return true;
    }
    if (auto scene = scene_serializer_.Deserialize(scene_folder_ / (name + ".msc")))
    {
        scenes_.emplace(name, scene_serializer_.Deserialize(scene_folder_ / (name + ".msc")));
        GetScene(name).OnActivate();
        return true;
    }
    return false;
}

void meteor::SceneManager::SaveScene(const std::string& name)
{
    auto it = scenes_.find(name);
    if (it == scenes_.end())
    {
        METEOR_CORE_WARN("Can't save scene");
        return;
    }
    it->second->OnDeactivate();
    scene_serializer_.Serialize(scene_folder_ / (name + ".msc"), *it->second);
}

void meteor::SceneManager::CreateScene(const std::string& name)
{
    scenes_[name] = std::make_unique<Scene>(name);
    GetScene(name).OnActivate();
}

void meteor::SceneManager::Clear()
{
    for (auto& scene : scenes_)
    {
        scene.second->OnDeactivate();
    }
    scenes_.clear();
}

meteor::Scene& meteor::SceneManager::GetScene(const std::string& name)
{
    METEOR_ASSERT(scenes_.contains(name), "Scene {0} is not loaded", name);
    return *scenes_[name];
}