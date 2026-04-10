#include "systems/TransformSystem.hpp"

void meteor::TransformSystem::PropagateTransform3D(GlobalTransform3DComponent &parent_gtf, std::vector<ecs::Entity> &children, ecs::World &world)
{
    for (auto child : children)
    {
        if (world.HasComponent<Transform3DComponent>(child) && 
            world.HasComponent<GlobalTransform3DComponent>(child))
        {
            auto& tf = world.GetComponent<Transform3DComponent>(child);
            auto& gtf = world.GetComponent<GlobalTransform3DComponent>(child);

            glm::vec3 scaled_position = parent_gtf.scale * tf.position;
            gtf.position = parent_gtf.rotation * scaled_position + parent_gtf.position;
            gtf.scale = tf.scale * parent_gtf.scale;
            gtf.rotation = parent_gtf.rotation * tf.rotation;

            if (auto* children = world.TryGetComponent<ChildrenComponent>(child))
            {
                PropagateTransform3D(gtf, children->children, world);
            }
        }
    }
}

void meteor::TransformSystem::OnUpdate(ecs::World &world, const float dt)
{
    auto view = world.View<Transform3DComponent, GlobalTransform3DComponent>();
    view.Each([&](ecs::Entity entity, Transform3DComponent& tf, GlobalTransform3DComponent& gtf)
    {
        // Filter for root entities without parent and with transform
        if (!world.HasComponent<ParentComponent>(entity))
        {
            // Root entity has global = local transform 
            gtf.position = tf.position;
            gtf.scale = tf.scale;
            gtf.rotation = tf.rotation;

            // Propagate recursive transform update from root entity
            if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
            {
                PropagateTransform3D(gtf, children->children, world);
            }
        }
    });
}