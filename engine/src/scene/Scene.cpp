#include "scene/Scene.hpp"
#include "Scene.hpp"

struct Pos
{
    int x;
};

void meteor::Scene::InitWorld()
{
    world_.RegisterComponent<Pos>();
    auto e = world_.Create();
    world_.AddComponent<Pos>(e);
}

meteor::Scene::Scene()
{
    InitWorld();
}

void meteor::Scene::OnUpdate()
{

}