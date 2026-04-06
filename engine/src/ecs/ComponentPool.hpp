#pragma once

#include "ecs/SparseSet.hpp"

namespace meteor::ecs::internal
{
    template<typename Component>
    class ComponentPool : public SparseSet
    {
    private:
        using PackedComponentsType = std::vector<Component>;

    public:
        using Iterator = PackedComponentsType::const_iterator;
    
    public:
        ComponentPool() = default;
        ComponentPool(ComponentPool&&) = default;

        ~ComponentPool() = default;

        template<typename... Args>
        void Emplace(Entity entity, Args&&... args)
        {
            auto it = Insert(entity);
            size_t index = it - SparseSet::begin();
            
            assert(index <= packed_components_.size() && "Packed components out of sync with sparse set");

            if (packed_components_.size() == index)
            {
                packed_components_.emplace_back(std::forward<Args>(args)...);
            }
            else 
            {
                packed_components_[index] = Component(std::forward<Args>(args)...);
            }
        }

        void Erase(Entity entity) override
        {
            size_t index = GetIndex(entity);
            if (Empty() || index == INVALID_INDEX) return;
            
            size_t last = Size() - 1;

            if (index != last)
            {
                packed_components_[index] = std::move(packed_components_[last]);
            }
            packed_components_.pop_back();
            SparseSet::Erase(entity);
        }

        void Sort() override
        {
            std::vector<Entity> old_packed(SparseSet::begin(), SparseSet::end());
            std::vector<bool> visited(old_packed.size(), false);

            SparseSet::Sort();

            for (size_t i = 0; i < old_packed.size(); i++)
            {
                if (visited[i]) continue;

                size_t j = i;
                while (GetIndex(old_packed[j]) != i)
                {
                    size_t dest = GetIndex(old_packed[j]);
                    std::swap(packed_components_[j], packed_components_[dest]);
                    visited[j] = true;
                    j = dest;
                }
                visited[j] = true;
            }
        }

        void Clear() noexcept override
        {
            SparseSet::Clear();
            packed_components_.clear();
        }

        [[nodiscard]] Iterator begin() const noexcept
        {
            return Iterator(packed_components_.data());
        }

        [[nodiscard]] Iterator end() const noexcept
        {
            return Iterator(packed_components_.data() + packed_components_.size());
        }

        [[nodiscard]] Component& Get(Entity entity)
        {
            size_t index = GetIndex(entity);
            if (index == INVALID_INDEX)
            {
                METEOR_CORE_ERROR("Entity {} not found in component pool", EntityId(entity));
                static Component fallback{};
                return fallback;
            }
            return packed_components_[index];
        }

        [[nodiscard]] const Component& Get(Entity entity) const
        {
            size_t index = GetIndex(entity);
            if (index == INVALID_INDEX)
            {
                METEOR_CORE_ERROR("Entity {} not found in component pool", EntityId(entity));
                static const Component fallback{};
                return fallback;
            }
            return packed_components_[index];
        }

        [[nodiscard]] Component* TryGet(Entity entity)
        {
            size_t index = GetIndex(entity);
            return (index != INVALID_INDEX) ? &packed_components_[index] : nullptr;
        }

        [[nodiscard]] const Component* TryGet(Entity entity) const
        {
            size_t index = GetIndex(entity);
            return (index != INVALID_INDEX) ? &packed_components_[index] : nullptr;
        }

        ComponentPool& operator=(ComponentPool&&) = default;

    private:
        PackedComponentsType packed_components_;
    };
}