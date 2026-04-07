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
    dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e){
        if (e.GetKeyCode() == GLFW_KEY_W)
        {
            printf("W");
            return true;
        }
        return false;
    });
}

void meteor::Scene::OnUpdate(const float dt)
{
    for (auto& module : modules_)
    {
        module->OnUpdate(dt);
    }
}
