#pragma once 

#include "systems/SystemBase.hpp"

namespace meteor
{
    class TransformSystem : public SystemBase
    {
    private:
        void PropagateTransform(const glm::mat4& parent_transform, ecs::Entity entity, ecs::World& world);

    public:
        TransformSystem() = default;

        void OnUpdate(ecs::World& world, const float dt) override;
    };
}