#pragma once

#include "core/UUID.hpp"

namespace meteor
{
    struct ChildrenComponent
    {
        std::vector<UUID> children;
    };
}