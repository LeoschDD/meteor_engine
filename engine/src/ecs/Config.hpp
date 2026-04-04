#pragma once

#include "ecs/Types.hpp"

namespace meteor::ecs
{
    inline constexpr Entity NULL_ENTITY = std::numeric_limits<Entity>::max();
    inline constexpr Entity MAX_ENTITIES = NULL_ENTITY;
    inline constexpr ComponentId MAX_COMPONENTS = std::numeric_limits<ComponentId>::max();
    inline constexpr Index NULL_INDEX = std::numeric_limits<Index>::max();
}