#pragma once

#include "ecs/ComponentPool.hpp"

namespace meteor::ecs
{
    template <typename... Cs>
    class View;

    inline std::atomic<ComponentId> next_component_id{0};

    template <typename C>
    inline ComponentId GetComponentId() noexcept
    {
        static const ComponentId component_id = next_component_id.fetch_add(1, std::memory_order_relaxed);
        return component_id;
    }

    class Registry
    {
        template <typename... Cs>
        friend class View;

    private:
        std::unordered_map<ComponentId, std::unique_ptr<ComponentPoolBase>> pools_;
        std::unordered_set<Entity> alive_;

        std::queue<Entity> available_;
        Entity next_id_{};

    private:
        template <typename C>
        ComponentPool<C>* GetComponentPool()
        {
            ComponentId component_id = GetComponentId<C>();

            if (pools_.contains(component_id))
            {
                return static_cast<ComponentPool<C>*>(pools_.at(component_id).get());
            }
            return nullptr;
        } 

        template <typename C>
        const ComponentPool<C>* GetComponentPool() const
        {
            ComponentId component_id = GetComponentId<C>();

            if (pools_.contains(component_id))
            {
                return static_cast<const ComponentPool<C>*>(pools_.at(component_id).get());
            }
            return nullptr;
        } 

    public:
        Registry() = default;
        ~Registry() = default;

        template <typename C>
        void RegisterComponent()
        {
            ComponentId component_id = GetComponentId<C>();

            if (component_id >= MAX_COMPONENTS)
            {
                METEOR_CORE_ERROR("Component limit reached");
                return;
            }
            if (!pools_.contains(component_id)) 
            {
                pools_[component_id] = std::make_unique<ComponentPool<C>>();
            }
        }

        Entity Create()
        {
            Entity entity = NULL_ENTITY;

            if (!available_.empty()) 
            {
                entity = available_.front();
                available_.pop();
            }
            else if (next_id_ < MAX_ENTITIES)
            {
                entity = next_id_++;
            }
            else
            {
                METEOR_CORE_ERROR("Entity limit reached");
                return entity;
            }
            alive_.emplace(entity);
            return entity;
        }

        void Erase(Entity entity)
        {
            if (alive_.contains(entity))
            {
                alive_.erase(entity);
                for (auto& [key, pool] : pools_)
                {
                    pool->Erase(entity);
                }
                available_.push(entity);
            }
        }

        template<typename C, typename... Args>
        void AddComponent(Entity entity, Args&&... args)
        {
            if (!Alive(entity)) return;
            if (auto* pool = GetComponentPool<C>())
            {
                pool->Emplace(entity, std::forward<Args>(args)...);
            }
            else
            {
                METEOR_CORE_WARN("Tried to add component that does not exist");
            }
        }

        template<typename C>
        void RemoveComponent(Entity entity)
        {	
            if (!Alive(entity)) return;
            if (auto* pool = GetComponentPool<C>())
            {
                pool->Erase(entity);
            }
            else
            {
                METEOR_CORE_WARN("Tried to remove component that does not exist");
            }
        }

        template<typename C>
        [[nodiscard]] C* GetComponent(Entity entity) noexcept
        {
            if (!Alive(entity)) return nullptr;
            if (auto* pool = GetComponentPool<C>())
            {
                return pool->GetComponent(entity);
            }
            return nullptr;
        }

        template<typename C>
        [[nodiscard]] const C* GetComponent(Entity entity) const noexcept
        {
            if (!Alive(entity)) return nullptr;
            if (const auto* pool = GetComponentPool<C>())
            {
                return pool->GetComponent(entity);
            }
            return nullptr;
        }

        [[nodiscard]] bool Alive(Entity entity) const noexcept
        {
            return alive_.contains(entity);
        }

        template <typename... Cs>
        ecs::View<Cs...> View()
        {
            return ecs::View<Cs...>(this);
        }
    };
}