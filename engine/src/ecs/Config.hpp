#pragma once

#include "ecs/Types.hpp"

namespace meteor::ecs
{
    inline constexpr EntityId NULL_ENTITY = std::numeric_limits<EntityId>::max();
    inline constexpr EntityId MAX_ENTITIES = NULL_ENTITY;
    inline constexpr ComponentId MAX_COMPONENTS = std::numeric_limits<ComponentId>::max();
    inline constexpr Index NULL_INDEX = std::numeric_limits<Index>::max();
}