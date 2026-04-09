#pragma once

#include "ecs/Ecs.hpp"

namespace meteor
{
    struct ParentComponent
    {
        ecs::Entity parent;
    };

    struct ChildrenComponent
    {
        std::vector<ecs::Entity> children;
    };
}