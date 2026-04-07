#include "scene/Scene.hpp"
#include "Scene.hpp"

meteor::Scene::Scene()
{}

void meteor::Scene::OnStart()
{
    started_ = true;

    for (auto& module : modules_)
    {
        module->OnStart();
    }
}

void meteor::Scene::OnActivate()
{
    for (auto& module : modules_)
    {
        module->OnActivate();
    }
}

void meteor::Scene::OnDeactivate()
{
    for (auto& module : modules_)
    {
        module->OnDeactivate();
    }
}

void meteor::Scene::OnEvent(Event& event)
{
    auto dispatcher = EventDispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e)
    {
        METEOR_CORE_INFO("Resize height: {0}", e.GetHeight());
        return true;
    });
}

void meteor::Scene::OnUpdate(const float dt)
{
    for (auto& module : modules_)
    {
        module->OnUpdate(dt);
    }
}
