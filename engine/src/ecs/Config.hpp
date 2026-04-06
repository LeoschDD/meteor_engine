#pragma once

#include "ecs/Types.hpp"

namespace meteor::ecs
{
    static constexpr Entity INVALID_ENTITY = std::numeric_limits<Entity>::max();
    static constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();
}