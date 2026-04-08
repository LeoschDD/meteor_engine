#include "scene/SceneManager.hpp"
#include "core/Core.hpp"

void meteor::SceneManager::LoadScene(const std::string& name)
{
    if (scenes_.contains(name)) 
    {
        METEOR_CORE_WARN("Tried to load scene {} that is already loaded", name);
        return;
    }
    scenes_.emplace(name, scene_serializer_.Deserialize(scene_folder_.append(name + ".msc")));
}

void meteor::SceneManager::SaveScene(const std::string& name)
{
}

void meteor::SceneManager::CreateScene(const std::string& name)
{
    scenes_[name] = std::make_unique<Scene>(name);
}

meteor::Scene& meteor::SceneManager::GetScene(const std::string& name)
{
    METEOR_ASSERT(scenes_.contains(name), "Scene {0} is not loaded", name);
    return *scenes_[name];
}
