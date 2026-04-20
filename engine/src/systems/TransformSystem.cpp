#include "systems/TransformSystem.hpp"
#include "components/TransformComponents.hpp"
#include "components/BasicComponents.hpp"
#include "core/Maths.hpp"

void meteor::TransformSystem::PropagateTransform(const glm::mat4& parent_transform, ecs::Entity entity, ecs::World& world)
{
    if (auto* transform = world.TryGetComponent<TransformComponent>(entity))
    {
        if (!world.HasComponent<GlobalTransformComponent>(entity))
        {
            world.AddComponent<GlobalTransformComponent>(entity);
        }
        auto& global_transform = world.GetComponent<GlobalTransformComponent>(entity).transform;
        auto transform_matrix = BuildTransformMatrix(transform->translation, transform->rotation, transform->scale);
        global_transform = parent_transform * transform_matrix;
    
        if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
        {
            for (auto child : children->children)
            {
                PropagateTransform(global_transform, child, world);
            }
        }
    }
}

void meteor::TransformSystem::OnUpdate(ecs::World& world, const float dt)
{
    world.View<TransformComponent>().Each([&](ecs::Entity entity, TransformComponent& transform)
    {
        auto* parent = world.TryGetComponent<ParentComponent>(entity);
        if (!(parent && world.HasComponent<TransformComponent>(parent->parent)))
        { 
            if (!world.HasComponent<GlobalTransformComponent>(entity))
            {
                world.AddComponent<GlobalTransformComponent>(entity);
            }
            auto& global_transform = world.GetComponent<GlobalTransformComponent>(entity).transform;
            global_transform = BuildTransformMatrix(transform.translation, transform.rotation, transform.scale);
            
            if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
            {
                for (auto child : children->children)
                {
                    PropagateTransform(global_transform, child, world);
                }
            }
        }
    });
}
