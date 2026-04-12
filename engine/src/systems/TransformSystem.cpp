#include "systems/TransformSystem.hpp"

void meteor::TransformSystem::PropagateTransform3D(Transform3DComponent& parent_transform, ecs::Entity entity, ecs::World &world, bool tree_dirty)
{
    if (auto* transform = world.TryGetComponent<Transform3DComponent>(entity))
    {
        bool dirty = tree_dirty || transform->dirty_;
        if (dirty)
        {
            glm::vec3 scaled_position = parent_transform.global_scale_ * transform->local_translation_;
            transform->global_translation_ = parent_transform.global_rotation_ * scaled_position + parent_transform.global_translation_;
            transform->global_rotation_ = parent_transform.global_rotation_ * transform->local_rotation_;
            transform->global_scale_ = transform->local_scale_ * parent_transform.global_scale_;
            transform->dirty_ = false;   
        }

        if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
        {
            for (auto child : children->children)
            {
                PropagateTransform3D(*transform, child, world, dirty);
            }
        }
    }
}

void meteor::TransformSystem::PropagateTransform2D(Transform2DComponent &parent_transform, ecs::Entity entity, ecs::World &world, bool tree_dirty)
{
    if (auto* transform = world.TryGetComponent<Transform2DComponent>(entity))
    {
        bool dirty = tree_dirty || transform->dirty_;
        if (dirty)
        {
            glm::vec2 scaled_pos = transform->local_translation_ * parent_transform.global_scale_;
            float cos = std::cos(parent_transform.global_rotation_);
            float sin = std::sin(parent_transform.global_rotation_);
            transform->global_translation_.x = parent_transform.global_translation_.x + scaled_pos.x * cos - scaled_pos.y * sin;
            transform->global_translation_.y = parent_transform.global_translation_.y + scaled_pos.x * sin + scaled_pos.y * cos;
            transform->global_rotation_ = parent_transform.global_rotation_ + transform->local_rotation_;
            transform->global_scale_ = transform->local_scale_ * parent_transform.global_scale_;   
            transform->dirty_ = false;       
        }

        if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
        {
            for (auto child : children->children)
            {
                PropagateTransform2D(*transform, child, world, dirty);
            }
        }
    }
}

void meteor::TransformSystem::OnUpdate(ecs::World &world, const float dt)
{
    auto view3d = world.View<Transform3DComponent>();
    view3d.Each([&](ecs::Entity entity, Transform3DComponent& transform)
    {
        auto* parent = world.TryGetComponent<ParentComponent>(entity);
        if (!(parent && world.HasComponent<Transform3DComponent>(parent->parent)))
        { 
            bool dirty = transform.dirty_;
            if (dirty)
            {
                transform.global_translation_ = transform.local_translation_;
                transform.global_rotation_ = transform.local_rotation_;
                transform.global_scale_ = transform.local_scale_;
                transform.dirty_ = false;
            }

            if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
            {
                for (auto child : children->children)
                {
                    PropagateTransform3D(transform, child, world, dirty);
                }
            }
        }
    });

    auto view2d = world.View<Transform2DComponent>();
    view2d.Each([&](ecs::Entity entity, Transform2DComponent& transform)
    {
        auto* parent = world.TryGetComponent<ParentComponent>(entity);
        if (!(parent && world.HasComponent<Transform2DComponent>(parent->parent)))
        {
            bool dirty = transform.dirty_;
            if (dirty)
            {
                transform.global_translation_ = transform.local_translation_;
                transform.global_rotation_ = transform.local_rotation_;
                transform.global_scale_ = transform.local_scale_;
                transform.dirty_ = false;
            }

            if (auto* children = world.TryGetComponent<ChildrenComponent>(entity))
            {
                for (auto child : children->children)
                {
                    PropagateTransform2D(transform, child, world, dirty);
                }
            }
        }
    });
}
