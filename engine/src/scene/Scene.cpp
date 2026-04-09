#include "scene/Scene.hpp"

meteor::Scene::Scene(const std::string& name)
    : name_(name)
{}

meteor::Scene::Scene(const std::string& name, ecs::World world)
    : name_(name)
    , world_(std::move(world))
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

}

void meteor::Scene::OnUpdate(const float dt)
{

}

void meteor::Scene::OnRender()
{
}