#pragma once

#include "ecs/ComponentPool.hpp"
#include "core/Log.hpp"

namespace meteor::ecs
{
    using namespace internal;

    class World
    {
    private:
        using PoolContainer = std::unordered_map<uint32_t, std::unique_ptr<SparseSet>>;
        using EntityQueue = std::queue<Entity>;
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
                entity = CreateEntity(next_id_++, 0);
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
            if (Valid(entity))
            {
                entities_.Erase(entity);
                for (auto& [key, pool] : pools_)
                {
                    pool->Erase(entity);
                }
                Entity next_version = EntityVersion(entity) + 1;
                if (next_version >= MAX_VERSION)
                {
                    METEOR_CORE_WARN("Entity {} version wrapped to 0", EntityId(entity));
                    next_version = 0;
                }
                recycled_.push(CreateEntity(EntityId(entity), next_version));
            }
        }

        [[nodiscard]] bool Valid(Entity entity) const
        {
            return entity != INVALID_ENTITY && entities_.Contains(entity);
        }

    private:
        EntityStorage entities_;
        PoolContainer pools_;

        EntityQueue recycled_;
        Entity next_id_{0};
    };
}
