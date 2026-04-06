#pragma once

#include "core/Headers.hpp"
#include "ecs/Entity.hpp"
#include "ecs/Config.hpp"

namespace meteor::ecs::internal
{
    class SparseSet
    {
    private:
        static constexpr uint16_t page_size = 1024;

        static_assert(page_size && ((page_size & (page_size - 1)) == 0), "Page size must be power of 2");

        using PageType = std::array<size_t, page_size>;
        using PackedType = std::vector<Entity>;
        using SparseType = std::vector<PageType*>;
    
    public:
        using Iterator = PackedType::const_iterator;

    private:
        [[nodiscard]] size_t GetPage(Entity entity) const noexcept
        {
            return static_cast<size_t>(entity / page_size);
        }

        [[nodiscard]] size_t GetPos(Entity entity) const noexcept
        {
            return static_cast<size_t>(entity & (page_size - 1));
        }

        [[nodiscard]] size_t* FindSparseSlot(Entity entity) const 
        {
            size_t page = GetPage(entity);
            size_t pos = GetPos(entity);

            if (sparse_.size() <= page || !sparse_[page])
            {
                return nullptr;
            }
            return &sparse_[page]->at(pos);
        }
        
        [[nodiscard]] size_t& EnsureSparseSlot(Entity entity)
        {
            size_t page = GetPage(entity);
            size_t pos = GetPos(entity);

            if (sparse_.size() <= page)
            {
                sparse_.resize(page + 1, nullptr);
            }
            if (!sparse_[page])
            {
                sparse_[page] = new PageType();
                sparse_[page]->fill(INVALID_INDEX);
            }
            return sparse_[page]->at(pos);
        }

        void DeletePages() noexcept
        {
            for (auto& page : sparse_)
            {
                delete page;
            }
        }

        void RebuildSparse()
        {
            DeletePages();
            sparse_.clear();
            
            size_t i = 0;
            for (Entity e : packed_)
            {
                EnsureSparseSlot(e) = i;
                i++;
            }
        }
    
    protected:
        virtual Iterator EmplaceEntity(Entity entity)
        {
            auto& slot = EnsureSparseSlot(entity);

            if (slot == INVALID_INDEX)
            {
                packed_.push_back(entity);
                slot = packed_.size() - 1;
            }
  
            return Iterator(packed_.data() + slot);
        }

        [[nodiscard]] size_t GetIndex(Entity entity) const
        {
            auto* slot = FindSparseSlot(entity);
            return slot ? *slot : INVALID_INDEX;
        }

    public:
        SparseSet() = default;

        SparseSet(const SparseSet&) = delete;

        SparseSet(SparseSet&& other) noexcept
            : packed_(std::move(other.packed_))
            , sparse_(std::move(other.sparse_)) {}

        virtual ~SparseSet()
        {
            DeletePages();
        }

        virtual void Erase(Entity entity)
        {
            auto* slot = FindSparseSlot(entity);
            if (!slot || Empty() || *slot == INVALID_INDEX) return;
            
            size_t last = Size() - 1;

            if (*slot != last)
            {
                Entity moved = packed_[last];
                packed_[*slot] = moved;

                auto& moved_slot = EnsureSparseSlot(moved);
                moved_slot = *slot;
            }
            packed_.pop_back();
            *slot = INVALID_INDEX;
        }

        void Sort()
        {
            std::sort(packed_.begin(), packed_.end(), [&](Entity a, Entity b){
                return a > b;
            });
            RebuildSparse();
        }

        void Clear() noexcept
        {
            DeletePages();
            packed_.clear();
            sparse_.clear();
        }

        [[nodiscard]] bool Contains(Entity entity) const
        {
            auto* slot = FindSparseSlot(entity); 
            return slot ? *slot != INVALID_INDEX : false;
        }

        [[nodiscard]] bool Empty() const noexcept
        {
            return packed_.empty();
        }

        [[nodiscard]] size_t Size() const noexcept
        {
            return packed_.size();
        }

        [[nodiscard]] Entity At(size_t index) const
        {
            return packed_.at(index);
        }

        [[nodiscard]] Entity operator[](size_t index) const
        {
            return packed_[index];
        }

        SparseSet& operator=(const SparseSet&) = delete;

        SparseSet& operator=(SparseSet&& other) noexcept
        {
            DeletePages();
            packed_ = std::move(other.packed_);
            sparse_ = std::move(other.sparse_);
            return *this;
        }

        [[nodiscard]] Iterator begin() const noexcept
        {
            return Iterator(packed_.data());
        }

        [[nodiscard]] Iterator end() const noexcept
        {
            return Iterator(packed_.data() + packed_.size());
        }

    private:
        PackedType packed_;
        SparseType sparse_;
    };
}