#include "scene/Scene.hpp"
#include "systems/TransformSystem.hpp"

meteor::Scene::Scene(const std::string& name)
    : name_(name)
{}

meteor::Scene::Scene(const std::string& name, ecs::World world)
    : name_(name)
    , world_(std::move(world))
{}

void meteor::Scene::OnCreate()
{
    if (created_) return;
    created_ = true;

    AddSystem(std::make_unique<TransformSystem>());
}

void meteor::Scene::OnDestroy()
{
    created_ = false;

    for (auto& system : systems_)
    {
        system->OnDeactivate(world_);
        system->OnDestroy(world_);
    }
}

void meteor::Scene::OnActivate()
{

}

void meteor::Scene::OnDeactivate()
{

}

void meteor::Scene::OnEvent(Event& event)
{
    for (auto& system : systems_)
    {
        system->OnEvent(world_, event);
    }
}

void meteor::Scene::OnUpdate(const float dt)
{
    for (auto& system : systems_)
    {
        system->OnUpdate(world_, dt);
    }
}

void meteor::Scene::OnRender()
{
}

void meteor::Scene::AddSystem(std::unique_ptr<SystemBase> system)
{
    systems_.push_back(std::move(system));
    systems_.back()->OnCreate(world_);
    systems_.back()->OnActivate(world_);
}