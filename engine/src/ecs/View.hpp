#pragma once

#include "ecs/Types.hpp"
#include "ecs/Registry.hpp"

namespace meteor::ecs
{
    template <typename... Cs>
    class View
    {
        friend class Registry;
    private:
        std::tuple<ComponentPool<Cs>*...> pools_;
        bool valid_{false};

    private:
        ComponentPoolBase* FindSmallestPool()
        {
            if constexpr (sizeof...(Cs) == 0) return nullptr;
            if (!valid_) return nullptr;

            std::array<ComponentPoolBase*, sizeof...(Cs)> bases{std::get<ComponentPool<Cs>*>(pools_)...};
            return *std::min_element(bases.begin(), bases.end(),
                [](ComponentPoolBase* a, ComponentPoolBase* b)
                {
                    return a->GetEntities().size() < b->GetEntities().size();
                }
            );
        }

        bool PoolsContain(Entity entity) const
        {
            return (std::get<ComponentPool<Cs>*>(pools_)->Contains(entity) && ...);
        }

        View(Registry* registry)
            : pools_{registry->GetComponentPool<Cs>()...}
            , valid_{(... && (std::get<ComponentPool<Cs>*>(pools_) != nullptr))}
        {
            if (!valid_)
            {
                METEOR_CORE_ERROR("View called with unregistered component");
            }
        }

    public:
        template <typename Fn>
        void Each(Fn&& fn)
        {
            auto* smallest = FindSmallestPool();
            if (!smallest) return;

            const auto& entities = smallest->GetEntities();

            for (size_t i = 0; i < entities.size(); ++i)
            {
                Entity entity = entities[i];
                if (PoolsContain(entity))
                {
                    fn(entity, *std::get<ComponentPool<Cs>*>(pools_)->GetComponent(entity)...);
                }
            }
        }
    };
}
