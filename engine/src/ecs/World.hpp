#pragma once

#include "ecs/ComponentPool.hpp"
#include "core/Log.hpp"
#include "ecs/View.hpp"

namespace meteor::ecs
{
    class World
    {
    private:
        using PoolMap = std::unordered_map<ComponentId, std::unique_ptr<internal::SparseSet>>;
        using EntityQueue = std::queue<Entity>;

    public:
        using Iterator = internal::EntitySparseSet::Iterator;

    private:
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
                pools_[id] = std::make_unique<internal::ComponentPool<Component>>();
            }
        }

        template<typename Component>
        [[nodiscard]] internal::ComponentPool<Component>* EnsurePool()
        {
            ComponentId id = GetComponentId<Component>();
            if (!pools_.contains(id)) 
            {
                RegisterComponent<Component>();
            }
            if (!pools_.contains(id)) 
            {
                METEOR_CORE_ERROR("Component registration failed");
                return nullptr;
            }
            return static_cast<internal::ComponentPool<Component>*>(pools_.at(id).get());
        }

        template<typename Component>
        [[nodiscard]] internal::ComponentPool<Component>* FindPool()
        {
            ComponentId id = GetComponentId<Component>();
            if (!pools_.contains(id)) return nullptr;
            return static_cast<internal::ComponentPool<Component>*>(pools_.at(id).get());
        }

    public:
        World() = default;
        World(const World&) = delete;
        World& operator=(const World&) = delete;
        World(World&&) = default;
        World& operator=(World&&) = default;

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
                recycled_.push(internal::CreateEntity(EntityId(entity), next_version));
            }
        }

        template<typename Component, typename... Args>
        void AddComponent(Entity entity, Args&&... args)
        {
            auto* pool = EnsurePool<Component>();
            if (pool) pool->Emplace(entity, std::forward<Args>(args)...);
        }

        template<typename Component>
        void EraseComponent(Entity entity)
        {
            auto* pool = FindPool<Component>();
            if (pool) pool->Erase(entity);
        }

        template<typename Component>
        [[nodiscard]] Component& GetComponent(Entity entity)
        {
            auto* pool = EnsurePool<Component>();
            METEOR_CORE_ASSERT(pool, "Component not registered");
            return pool->Get(entity);
        }

        template<typename Component>
        [[nodiscard]] Component* TryGetComponent(Entity entity)
        {
            auto* pool = FindPool<Component>();
            if (pool) return pool->TryGet(entity);
            return nullptr;
        }

        template<typename Component>
        [[nodiscard]] bool HasComponent(Entity entity)
        {
            auto* pool = FindPool<Component>();
            if (pool) return pool->Contains(entity);
            return false;
        }

        template<typename... Components>
        [[nodiscard]] ecs::View<Components...> View()
        {
            return ecs::View<Components...>(EnsurePool<Components>()...);
        }

        [[nodiscard]] bool Valid(Entity entity) const
        {
            return entity != INVALID_ENTITY && entities_.Contains(entity);
        }

        [[nodiscard]] bool Empty() const noexcept
        {
            return entities_.Empty();
        }

        [[nodiscard]] size_t Size() const noexcept
        {
            return entities_.Size();
        }

        void Clear() noexcept
        {
            entities_.Clear();
            for (auto& pool : pools_)
            {
                pool.second->Clear();
            }
            next_id_ = 0;
            recycled_ = EntityQueue{};
        }

        [[nodiscard]] Iterator begin() const noexcept
        {
            return entities_.begin();
        }

        [[nodiscard]] Iterator end() const noexcept
        {
            return entities_.end();
        }

    private:
        internal::EntitySparseSet entities_;
        PoolMap pools_;

        EntityQueue recycled_;
        Entity next_id_{0};
    };
}
