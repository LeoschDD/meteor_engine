#pragma once

#include "ecs/Ecs.hpp"

namespace meteor
{
    ////////////////////////////////////////////////////
    // Basic hierarchy components //////////////////////
    ////////////////////////////////////////////////////

    struct ParentComponent
    {
        ecs::Entity parent;
    };

    struct ChildrenComponent
    {
        std::vector<ecs::Entity> children;
    };
}