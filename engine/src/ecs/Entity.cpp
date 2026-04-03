#include "ecs/Entity.hpp"
#include "ecs/Registry.hpp"

meteor::ecs::Entity::Entity(EntityId entity_id, Registry* registry)
    : entity_id_(entity_id)
    , registry_(registry)
{
}

template <typename C, typename... Args>
inline void meteor::ecs::Entity::Add(Args &&...args)
{
}

template <typename C>
inline void meteor::ecs::Entity::Remove()
{
}

void meteor::ecs::Entity::Destroy()
{
}

template <typename C>
C* meteor::ecs::Entity::Get() noexcept
{
    return nullptr;
}

template <typename C>
const C* meteor::ecs::Entity::Get() const noexcept
{
    return nullptr;
}

bool meteor::ecs::Entity::Valid() const noexcept
{
    return false;
}

meteor::ecs::EntityId meteor::ecs::Entity::Id() const
{
    return EntityId();
}
