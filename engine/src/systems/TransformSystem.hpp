#pragma once 

#include "systems/SystemBase.hpp"
#include "components/TransformComponents.hpp"
#include "components/BasicComponents.hpp"

namespace meteor
{
    class TransformSystem : public SystemBase
    {
    private:
        void PropagateTransform3D(GlobalTransform3DComponent& parent_gtf, std::vector<ecs::Entity>& children, ecs::World& world);

    public:
        void OnUpdate(ecs::World& world, const float dt) override;
    };
}