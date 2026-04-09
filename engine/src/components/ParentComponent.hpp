#pragma once

#include "ecs/Ecs.hpp"

namespace meteor
{
    struct ParentComponent
    {
        ecs::Entity parent;
    };
}