#include "scene/Scene.hpp"
#include "Scene.hpp"

meteor::Scene::Scene()
{}

void meteor::Scene::OnStart()
{
    if (started_) return;
    started_ = true;
}

void meteor::Scene::OnActivate()
{

}

void meteor::Scene::OnDeactivate()
{

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

}

void meteor::Scene::OnRender()
{
}
