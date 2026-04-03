#pragma once

#include "ecs/ComponentPool.hpp"

namespace meteor::ecs
{
    class Registry
    {
    private:
        std::vector<std::unique_ptr<ComponentPoolBase>> pools_;
        std::queue<EntityId> available_;
        std::unordered_map<EntityId, EntityId> alive_;

    public:
        Registry()
        {
            for (EntityId entity_id = 0; entity_id < MAX_ENTITIES; entity_id++) 
            {
                available_.push(entity_id);
            }
        }

        ~Registry() = default;

        template <typename C>
        void RegisterComponent()
        {
            pools_.emplace_back<ComponentPool<C>>();
        }

        [[nodiscard]] EntityId Create()
        {
            if (available_.empty()) 
            {
                METEOR_CORE_WARN("Entity limit reached");
                return NULL_ENTITY;
            }

            EntityId entity_id = available_.front();
            available_.pop();
            return entity_id;
        }
    };
}