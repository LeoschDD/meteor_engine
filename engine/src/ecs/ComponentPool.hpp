#pragma once

#include "ecs/Types.hpp"
#include "ecs/Config.hpp"
#include "core/Log.hpp"

namespace meteor::ecs
{
    class ComponentPoolBase
    {
    public:
        virtual ~ComponentPoolBase() = default;
        virtual void Remove(EntityId entity_id) = 0;
    };

    template <typename C>
    class ComponentPool : ComponentPoolBase
    {
    private:
        static constexpr uint16_t PAGE_SIZE = 4096;
        using Page = std::array<Index, PAGE_SIZE>;

        std::vector<C> components_;
        std::vector<EntityId> entities_;
        std::vector<std::unique_ptr<Page>> indices_;

    private:
        Index& EnsureIndexSlot(EntityId entity_id)
        {
            const size_t page =  entity_id / PAGE_SIZE;
            const size_t index = entity_id & (PAGE_SIZE - 1);
            
            if (indices_.size() <= page) indices_.resize(page + 1);

            if (!indices_[page]) 
            {
                indices_[page] = std::make_unique<Page>();
                indices_[page]->fill(NULL_INDEX);
            }
            return (*indices_[page])[index];
        }

        Index* FindIndexSlot(EntityId entity_id)
        {
            const size_t page =  entity_id / PAGE_SIZE;
            const size_t index = entity_id & (PAGE_SIZE - 1);
            
            if (indices_.size() <= page || !indices_[page]) return nullptr;
            Index& slot = (*indices_[page])[index];
            return (slot == NULL_INDEX) ? nullptr : &slot;
        }

        const Index* FindIndexSlot(EntityId entity_id) const
        {
            const size_t page =  entity_id / PAGE_SIZE;
            const size_t index = entity_id & (PAGE_SIZE - 1);
            
            if (indices_.size() <= page || !indices_[page]) return nullptr;
            const Index& slot = (*indices_[page])[index];
            return (slot == NULL_INDEX) ? nullptr : &slot;
        }

    public:
        ComponentPool() = default;
        ~ComponentPool() = default;
    
        template <typename... Args>
        void Emplace(EntityId entity_id, Args&&... args) 
        {
            Index& index = EnsureIndexSlot(entity_id);

            if (index == NULL_INDEX)
            {
                if (components_.size() >= NULL_INDEX)
                {
                    METEOR_CORE_ERROR("Component pool capacity exceeded");
                    return;
                }
                components_.emplace_back(std::forward<Args>(args)...);
                entities_.push_back(entity_id);
                index = static_cast<Index>(components_.size()) - 1;
            }
            else 
            {
                components_[index] = C(std::forward<Args>(args)...);				
            }            
        }

        void Remove(EntityId entity_id) override
        {
            Index* index = FindIndexSlot(entity_id);
            if (!index || components_.empty()) return;
            
            Index last = static_cast<Index>(components_.size()) - 1;

            if (*index != last)
            {
                EntityId moved = entities_[last];
                components_[*index] = std::move(components_[last]);
                entities_[*index] = moved;

                const size_t moved_page =  moved / PAGE_SIZE;
                const size_t moved_index = moved & (PAGE_SIZE - 1);

                (*indices_[moved_page])[moved_index] = *index;
            }
            components_.pop_back();
            entities_.pop_back();
            *index = NULL_INDEX;
        }

        C* GetComponent(EntityId entity_id)
        {
            Index* index = FindIndexSlot(entity_id);
            return index ? &components_[*index] : nullptr;
        }

        const C* GetComponent(EntityId entity_id) const
        {
            const Index* index = FindIndexSlot(entity_id);
            return index ? &components_[*index] : nullptr;
        }

        const std::vector<C>& GetComponents() const { return components_; } 
        const std::vector<EntityId>& GetEntities() const { return entities_; }
    };
}