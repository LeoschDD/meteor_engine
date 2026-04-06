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

    private:
    
    public:
        ComponentPool() = default;
        ~ComponentPool() = default;

        template<typename... Args>
        void Emplace(Entity entity, Args&&... args)
        {
            auto it = SparseSet::EmplaceEntity(entity);
            size_t index = it - SparseSet::begin();
            
            assert(index <= packed_components_.size());

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

        void Sort()
        {
            std::vector<Entity> packed_copy(SparseSet::begin(), SparseSet::end());
            PackedComponentsType temp(Size());

            SparseSet::Sort();
            for (size_t i = 0; i < packed_copy.size(); i++)
            {
                Entity entity = packed_copy[i];
                size_t index = GetIndex(entity);
                if (index == INVALID_INDEX) continue;
                temp[index] = std::move(packed_components_[i]);
            }
            packed_components_.swap(temp);
        }

        void Clear() noexcept override
        {
            SparseSet::Clear();
            packed_components_.clear();
        }

        [[nodiscard]] Iterator begin() const noexcept override
        {
            return Iterator(packed_components_.data());
        }

        [[nodiscard]] Iterator end() const noexcept override
        {
            return Iterator(packed_components_.data() + packed_components_.size());
        }

    private:
        PackedComponentsType packed_components_;
    };
}