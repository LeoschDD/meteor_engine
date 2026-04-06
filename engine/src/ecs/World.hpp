#pragma once

#include "ecs/ComponentPool.hpp"

namespace meteor::ecs
{
    class World
    {
    private:
        using EntityContainer = std::vector<Entity>;
        using PoolContainer = std::unordered_map<uint32_t, std::unique_ptr<internal::SparseSet>>;

    private:

    public:
        World() = default;

        Entity Create()
        {
            Entity entity = internal::CreateEntity(1, 1);
            entities_.push_back(entity);
            return entity;
        }

        template<typename Component>
        void RegisterComponent()
        {
            pools_.emplace<internal::ComponentPool<Component>>(GetComponentId<Component>());
        }

    private:
        EntityContainer entities_;
        PoolContainer pools_;
    };
}
