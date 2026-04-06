#pragma once

#include "ecs/ComponentPool.hpp"
#include "core/Log.hpp"

namespace meteor::ecs
{
    class World
    {
    private:
        using PoolContainer = std::unordered_map<uint32_t, std::unique_ptr<internal::SparseSet>>;

    public:
        World() = default;

        template <typename Component>
        void RegisterComponent()
        {
            ComponentId id = GetComponentId<Component>();

            if (id >= MAX_COMPONENTS)
            {
                METEOR_CORE_ERROR("Component limit reached");
                return;
            }
            if (!pools_.contains(id)) 
            {
                pools_[id] = std::make_unique<ComponentPool<Component>>();
            }
        }

        Entity Create()
        {
            Entity entity = INVALID_ENTITY;

            if (!recycled_.empty()) 
            {
                entity = recycled_.front();
                recycled_.pop();
            }
            else if (next_id_ < MAX_ENTITY_ID)
            {
                entity = internal::CreateEntity(next_id_++, 0);
            }
            else
            {
                METEOR_CORE_ERROR("Entity limit reached");
                return entity;
            }
            entities_.Emplace(entity);
            return entity;
        }

        void Erase(Entity entity)
        {
            if (entities_.Contains(entity))
            {
                entities_.Erase(entity);
                for (auto& [key, pool] : pools_)
                {
                    pool->Erase(entity);
                }
                recycled_.push(internal::CreateEntity(EntityId(entity), EntityVersion(entity) + 1));
            }
        }


    private:
        internal::EntityStorage entities_;
        PoolContainer pools_;

        std::queue<Entity> recycled_;
        Entity next_id_;
    };
}
