#pragma once

#include "core/Headers.hpp"

#ifdef ECS_ENTITY_32
    #define ENTITY uint32_t
#else
    #define ENTITY uint64_t
#endif

namespace meteor::ecs
{
    using Entity = ENTITY;

    template<typename>
    struct EntityTraits;

    template<>
    struct EntityTraits<uint32_t> 
    {
        using entity_type = uint32_t;
        static constexpr entity_type entity_mask = 0xFFFFF;
        static constexpr entity_type version_mask = 0xFFF;
        static constexpr uint16_t version_shift = 20;
    };

    template<>
    struct EntityTraits<uint64_t> 
    {
        using entity_type = uint64_t;
        static constexpr entity_type entity_mask = 0xFFFFFFFF;
        static constexpr entity_type version_mask = 0xFFFFFFFF;
        static constexpr uint16_t version_shift = 32;
    };

    using Traits = EntityTraits<Entity>;

    [[nodiscard]] inline constexpr Entity EntityId(Entity entity) noexcept
    {
        return entity & Traits::entity_mask;
    }

    [[nodiscard]] inline constexpr Entity EntityVersion(Entity entity) noexcept
    {
        return (entity >> Traits::version_shift) & Traits::version_mask;
    }

    namespace internal
    {
        [[nodiscard]] inline constexpr Entity CreateEntity(Entity id, Entity version) noexcept
        {
            return (version << Traits::version_shift) | (id & Traits::entity_mask);
        }
    }

    using ComponentId = uint32_t;

    inline ComponentId next_component_id = 0;

    template<typename Component>
    [[nodiscard]] inline ComponentId GetComponentId() noexcept
    {
        static ComponentId id = next_component_id++;
        return id;
    }
}