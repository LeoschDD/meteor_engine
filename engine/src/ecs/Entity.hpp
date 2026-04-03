#pragma once

#include "ecs/Types.hpp"

namespace meteor::ecs
{
    class Registry;

    class Entity
    {
    private:
        EntityId entity_id_;
        Registry* registry_;

    public:
        Entity(EntityId entity_id, Registry* registry);
        ~Entity() = default;

        template<typename C, typename... Args>
        void Add(Args&&... args);

        template<typename C>
        void Remove();

        void Destroy();

        template<typename C>
        [[nodiscard]] C* Get() noexcept;

        template<typename C>
        [[nodiscard]] const C* Get() const noexcept;

        [[nodiscard]] bool Valid() const noexcept;

        EntityId Id() const;
    };
}

