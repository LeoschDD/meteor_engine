#pragma once

#include "ecs/Ecs.hpp"

namespace meteor
{
    struct ChildrenComponent
    {
        std::vector<ecs::Entity> children;
    };
}