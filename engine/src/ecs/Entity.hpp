#pragma once

#include "core/Headers.hpp"

namespace meteor::ecs
{
    using Entity = uint64_t;

    template<typename>
    struct EntityTraits;

    template<>
    struct EntityTraits<uint64_t> 
    {
        using value_type = uint64_t;
        using entity_type = uint64_t;
        using version_type = uint32_t;

        static constexpr entity_type entity_mask = 0xFFFFFFFF;
        static constexpr entity_type version_mask = 0xFFFFFFFF;
    };
}