#include "scene/SceneManager.hpp"
#include "core/Core.hpp"

void meteor::SceneManager::LoadScene(const std::string& name)
{
}

void meteor::SceneManager::SaveScene(const std::string& name)
{
}

void meteor::SceneManager::CreateScene(const std::string& name)
{
    scenes_[name] = std::make_unique<Scene>();
}

meteor::Scene& meteor::SceneManager::GetScene(const std::string& name)
{
    METEOR_ASSERT(scenes_.contains(name), "Scene {0} is not loaded", name);
    return *scenes_[name];
}
