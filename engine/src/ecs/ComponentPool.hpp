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
        virtual void Erase(Entity entity) = 0;
        virtual bool Contains(Entity entity) const = 0;
        virtual const std::vector<Entity>& GetEntities() const = 0;
    };

    template <typename C>
    class ComponentPool : public ComponentPoolBase
    {
    private:
        static constexpr uint16_t PAGE_SIZE = 4096;
        using Page = std::array<Index, PAGE_SIZE>;

        std::vector<C> components_;
        std::vector<Entity> entities_;
        std::vector<std::unique_ptr<Page>> indices_;

    private:
        Index& EnsureIndexSlot(Entity entity)
        {
            const size_t page =  entity / PAGE_SIZE;
            const size_t index = entity & (PAGE_SIZE - 1);
            
            if (indices_.size() <= page) indices_.resize(page + 1);

            if (!indices_[page]) 
            {
                indices_[page] = std::make_unique<Page>();
                indices_[page]->fill(NULL_INDEX);
            }
            return (*indices_[page])[index];
        }

        Index* FindIndexSlot(Entity entity)
        {
            const size_t page =  entity / PAGE_SIZE;
            const size_t index = entity & (PAGE_SIZE - 1);
            
            if (indices_.size() <= page || !indices_[page]) return nullptr;
            Index& slot = (*indices_[page])[index];
            return (slot == NULL_INDEX) ? nullptr : &slot;
        }

        const Index* FindIndexSlot(Entity entity) const
        {
            const size_t page =  entity / PAGE_SIZE;
            const size_t index = entity & (PAGE_SIZE - 1);
            
            if (indices_.size() <= page || !indices_[page]) return nullptr;
            const Index& slot = (*indices_[page])[index];
            return (slot == NULL_INDEX) ? nullptr : &slot;
        }

    public:
        ComponentPool() = default;
        ~ComponentPool() = default;
    
        template <typename... Args>
        void Emplace(Entity entity, Args&&... args) 
        {
            Index& index = EnsureIndexSlot(entity);

            if (index == NULL_INDEX)
            {
                if (components_.size() >= NULL_INDEX)
                {
                    METEOR_CORE_ERROR("Component pool capacity exceeded");
                    return;
                }
                components_.emplace_back(std::forward<Args>(args)...);
                entities_.push_back(entity);
                index = static_cast<Index>(components_.size()) - 1;
            }
            else 
            {
                components_[index] = C(std::forward<Args>(args)...);				
            }            
        }

        void Erase(Entity entity) override
        {
            Index* index = FindIndexSlot(entity);
            if (!index || components_.empty()) return;
            
            Index last = static_cast<Index>(components_.size()) - 1;

            if (*index != last)
            {
                Entity moved = entities_[last];
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

        C* GetComponent(Entity entity)
        {
            Index* index = FindIndexSlot(entity);
            return index ? &components_[*index] : nullptr;
        }

        const C* GetComponent(Entity entity) const
        {
            const Index* index = FindIndexSlot(entity);
            return index ? &components_[*index] : nullptr;
        }

        bool Contains(Entity entity) const override { return FindIndexSlot(entity); }
        const std::vector<C>& GetComponents() const { return components_; } 
        const std::vector<Entity>& GetEntities() const override { return entities_; }
    };
}