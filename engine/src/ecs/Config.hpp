#pragma once

#include "ecs/Types.hpp"

namespace meteor::ecs
{
    static constexpr Entity INVALID_ENTITY = std::numeric_limits<Entity>::max();
    static constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

    static constexpr Entity MAX_ENTITY_ID = Traits::entity_mask;
    static constexpr Entity MAX_VERSION = Traits::version_mask;
    static constexpr ComponentId MAX_COMPONENTS = std::numeric_limits<ComponentId>::max();
}