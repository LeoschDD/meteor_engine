#pragma once

#include "ecs/ComponentPool.hpp"
#include "ecs/Entity.hpp"

namespace meteor::ecs
{
    inline std::atomic<ComponentId> next_component_id{0};

    template <typename C>
    inline ComponentId GetComponentId() noexcept
    {
        static const ComponentId component_id = next_component_id.fetch_add(1, std::memory_order_relaxed);
        return component_id;
    }

    class Registry
    {
    private:
        std::unordered_map<ComponentId, std::unique_ptr<ComponentPoolBase>> pools_;
        std::unordered_set<EntityId> alive_;

        std::queue<EntityId> available_;
        EntityId next_id_{};

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
            EntityId entity_id = NULL_ENTITY;

            if (!available_.empty()) 
            {
                entity_id = available_.front();
                available_.pop();
            }
            else if (next_id_ < MAX_ENTITIES)
            {
                entity_id = next_id_++;
            }
            else
            {
                METEOR_CORE_ERROR("Entity limit reached");
                return Entity(entity_id, this);
            }
            alive_.emplace(entity_id);
            return Entity(entity_id, this);
        }

        void Erase(EntityId entity_id)
        {
            if (alive_.contains(entity_id))
            {
                alive_.erase(entity_id);
                for (auto& [key, pool] : pools_)
                {
                    pool->Erase(entity_id);
                }
                available_.push(entity_id);
            }
        }

        template<typename C, typename... Args>
        void AddComponent(EntityId entity_id, Args&&... args)
        {
            if (!Alive(entity_id)) return;
            if (auto* pool = GetComponentPool<C>())
            {
                pool->Emplace(entity_id, std::forward<Args>(args)...);
            }
            else
            {
                METEOR_CORE_WARN("Tried to add component that does not exist");
            }
        }

        template<typename C>
        void RemoveComponent(EntityId entity_id)
        {	
            if (!Alive(entity_id)) return;
            if (auto* pool = GetComponentPool<C>())
            {
                pool->Erase(entity_id);
            }
            else
            {
                METEOR_CORE_WARN("Tried to remove component that does not exist");
            }
        }

        template<typename C>
        [[nodiscard]] C* GetComponent(EntityId entity_id) noexcept
        {
            if (!Alive(entity_id)) return nullptr;
            if (auto* pool = GetComponentPool<C>())
            {
                return pool->GetComponent(entity_id);
            }
            return nullptr;
        }

        template<typename C>
        [[nodiscard]] const C* GetComponent(EntityId entity_id) const noexcept
        {
            if (!Alive(entity_id)) return nullptr;
            if (const auto* pool = GetComponentPool<C>())
            {
                return pool->GetComponent(entity_id);
            }
            return nullptr;
        }

        [[nodiscard]] bool Alive(EntityId entity_id) const noexcept
        {
            return alive_.contains(entity_id);
        }
    };

    inline Entity::Entity(EntityId entity_id, Registry* registry)
        : entity_id_(entity_id)
        , registry_(registry)
    {}

    template<typename C, typename... Args>
    inline void Entity::Add(Args&&... args)
    {
        registry_->AddComponent<C>(entity_id_, std::forward<Args>(args)...);
    }

    template<typename C>
    inline void Entity::Remove()
    {
        registry_->RemoveComponent<C>(entity_id_);
    }

    inline void Entity::Destroy()
    {
        registry_->Erase(entity_id_);
    }

    template<typename C>
    [[nodiscard]] inline C* Entity::Get() noexcept
    {
        return registry_->GetComponent<C>(entity_id_);
    }

    template<typename C>
    [[nodiscard]] inline const C* Entity::Get() const noexcept
    {
        return registry_->GetComponent<C>(entity_id_);
    }

    [[nodiscard]] inline bool Entity::Alive() const noexcept
    {
        return registry_->Alive(entity_id_);
    }

    inline EntityId Entity::Id() const {return entity_id_;}
}