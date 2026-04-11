#include "systems/TransformSystem.hpp"

void meteor::TransformSystem::PropagateTransform3D(GlobalTransform3DComponent& parent_global_transform, ecs::Entity entity, ecs::World &world)
{

    auto* transform = world.TryGetComponent<Transform3DComponent>(entity);
    auto* global_transform = world.TryGetComponent<GlobalTransform3DComponent>(entity);
    
    if (transform && global_transform)
    {
        auto& transform = world.GetComponent<Transform3DComponent>(entity);
        auto& global_transform = world.GetComponent<GlobalTransform3DComponent>(entity);

        glm::vec3 scaled_position = parent_global_transform.scale * transform.position;
        global_transform.position = parent_global_transform.rotation * scaled_position + parent_global_transform.position;
        global_transform.scale = transform.scale * parent_global_transform.scale;
        global_transform.rotation = parent_global_transform.rotation * transform.rotation;

        if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
        {
            for (auto child : children->children)
            {
                PropagateTransform3D(global_transform, child, world);
            }
        }
    }
}

void meteor::TransformSystem::OnUpdate(ecs::World &world, const float dt)
{
    auto view = world.View<Transform3DComponent, GlobalTransform3DComponent>();
    view.Each([&](ecs::Entity entity, Transform3DComponent& transform, GlobalTransform3DComponent& global_transform)
    {
        // Entity is transform root if it has no parent or parent lacks transform component
        auto* parent = world.TryGetComponent<ParentComponent>(entity);
        bool root = !(parent  
            && world.HasComponent<Transform3DComponent>(parent->parent)
            && world.HasComponent<GlobalTransform3DComponent>(parent->parent));
        
        if (root)
        {
            // Root entity has global = local transform 
            global_transform.position = transform.position;
            global_transform.scale = transform.scale;
            global_transform.rotation = transform.rotation;

            // Propagate recursive transform update from root entity
            if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
            {
                for (auto child : children->children)
                {
                    PropagateTransform3D(global_transform, child, world);
                }
            }
        }
    });
}