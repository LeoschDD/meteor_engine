#pragma once

#include "core/Headers.hpp"
#include "ecs/Entity.hpp"
#include "ecs/Config.hpp"
#include <type_traits>

namespace meteor::ecs::internal
{
    template<typename Container>
    class SparseSetIterator
    {
    public:
        using ValueType = typename Container::value_type;
        using PointerType = typename Container::const_pointer;
        using ReferenceType = typename Container::const_reference;
        using DifferenceType = typename Container::difference_type;

    public:
        constexpr SparseSetIterator() noexcept
            : ptr_{}
            , offset_{} {}

        constexpr SparseSetIterator(PointerType ptr, DifferenceType index) noexcept
            : ptr_(ptr)
            , offset_(index) {}

        constexpr SparseSetIterator& operator++() noexcept
        {
            offset_++;
            return *this;
        }

        constexpr SparseSetIterator operator++(int) noexcept
        {
            SparseSetIterator it = *this;
            ++(*this);
            return it;
        }

        constexpr SparseSetIterator& operator--() noexcept
        {
            offset_--;
            return *this;
        }

        constexpr SparseSetIterator operator--(int) noexcept
        {
            SparseSetIterator it = *this;
            --(*this);
            return it;
        }

        [[nodiscard]] constexpr ReferenceType operator[](DifferenceType index) noexcept
        {
            return (*ptr_)[static_cast<Contaienr::size_type>(index() - index)]
        }

        [[nodiscard]] constexpr PointerType operator->() noexcept
        {
            return &(*ptr_)[index()];
        }

        [[nodiscard]] constexpr ReferenceType operator*() noexcept
        {
            return (*ptr_)[index()];
        }

        [[nodiscard]] constexpr bool operator==(const SparseSetIterator& other) const noexcept
        {
            return offset_ == other.offset_;
        }

        [[nodiscard]] constexpr bool operator!=(const SparseSetIterator& other) const noexcept
        {
            return offset_ != other.offset_;
        }

        [[nodiscard]] constexpr DifferenceType Index() const noexcept
        {
            return offset_ - 1;
        }

    private:
        PointerType ptr_;
        DifferenceType offset_;
    };

    class SparseSet
    {
    private:
        static constexpr uint16_t page_size = 1024;

        static_assert(page_size && ((page_size & (page_size - 1)) == 0), "Page size must be power of 2");

        using PageType = std::array<size_t, page_size>;
        using PackedType = std::vector<Entity>;
        using SparseType = std::vector<PageType*>;
    
    public:
        using Iterator = SparseSetIterator<PackedType>;

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
    
    protected:
        virtual Iterator EmplaceEntity(Entity entity)
        {
            auto& slot = EnsureSparseSlot(entity);

            if (slot == INVALID_INDEX)
            {
                packed_.push_back(entity);
                slot = packed_.size() - 1;
            }
            else 
            {
                packed_[slot] = entity;		
            }
        };

    public:
        SparseSet() = default;

        SparseSet(const SparseSet&) = delete;

        SparseSet(SparseSet&& other) noexcept
            : sparse_(std::move(other.sparse_))
            , packed_(std::move(other.packed_)) {}

        virtual ~SparseSet()
        {
            for (auto& page : sparse_)
            {
                delete page;
            }
        }

        virtual void Erase(Entity entity)
        {
            auto* slot = FindSparseSlot(entity);
            if (!slot || packed_.empty()) return;
            
            size_t last = packed_.size() - 1;

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

        [[nodiscard]] bool Contains(Entity entity) const
        {
            auto* slot = FindSparseSlot(entity); 
            return slot ? *slot != INVALID_INDEX : false;
        }

        [[nodiscard]] Iterator begin() const noexcept
        {
            return Iterator(packed_.data(), 0);
        }

        [[nodiscard]] Iterator end() const noexcept
        {
            return Iterator(packed_.data(), packed_.size());
        }

    private:
        PackedType packed_;
        SparseType sparse_;
    };
}