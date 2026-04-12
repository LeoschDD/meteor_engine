#pragma once 

#include "systems/SystemBase.hpp"
#include "components/TransformComponents.hpp"
#include "components/BasicComponents.hpp"

namespace meteor
{
    class TransformSystem : public SystemBase
    {
    private:
        void PropagateTransform3D(Transform3DComponent& parent_transform, 
            ecs::Entity entity, ecs::World& world, bool tree_dirty);

        void PropagateTransform2D(Transform2DComponent& parent_transform, 
            ecs::Entity entity, ecs::World& world, bool tree_dirty);

    public:
        void OnUpdate(ecs::World& world, const float dt) override;
    };
}