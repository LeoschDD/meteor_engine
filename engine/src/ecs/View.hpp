#pragma once

#include "ecs/Types.hpp"
#include "ecs/Registry.hpp"

namespace meteor::ecs
{
    template <typename... Cs>
    class View
    {
    private:
        Registry* registry_{nullptr};
        std::array<ComponentPoolBase*, sizeof...(Cs)> pools_;

    private:
        ComponentPoolBase* FindSmallestPool()
        {
            return *std::min_element(pools_.begin(), pools_.end(), 
                [](ComponentPoolBase* a, ComponentPoolBase* b) 
                {
                    return a->GetEntities().size() < b->GetEntities().size();
                }
            );
        }

        bool PoolsContain(EntityId entity_id) const
        {
            for (auto* pool : pools_)
            {
                if (!pool->Contains(entity_id)) return false;
            }
            return true;
        }

    public:
        View(Registry* registry)
            : registry_(registry)
            , pools_({registry_->GetComponentPool<Cs>()...})
        {}

        template <typename Fn>
        void Each(Fn&& fn)
        {
            const auto& entities = FindSmallestPool()->GetEntities();
            for (size_t i = 0; i < entities.size(); ++i)
            {
                EntityId entity_id = entities[i];
                if (PoolsContain(entity_id))
                {
                    fn(entity_id, *registry_->GetComponent<Cs>(entity_id)...);
                }
            }
        }
    };
}
