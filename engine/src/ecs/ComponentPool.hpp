#pragma once

#include "ecs/SparseSet.hpp"

namespace meteor::ecs::internal
{
    template<typename Container>
    class ComponentPoolIterator
    {
    public:
        using ValueType = typename Container::value_type;
        using PointerType = typename Container::const_pointer;
        using ReferenceType = typename Container::const_reference;
        using DifferenceType = typename Container::difference_type;

    public:
        constexpr ComponentPoolIterator() noexcept
            : ptr_{}
            , offset_{} {}

        constexpr ComponentPoolIterator(PointerType ptr, DifferenceType index) noexcept
            : ptr_(ptr)
            , offset_(index) {}

        constexpr ComponentPoolIterator& operator++() noexcept
        {
            offset_++;
            return *this;
        }

        constexpr ComponentPoolIterator operator++(int) noexcept
        {
            SparseSetIterator it = *this;
            ++(*this);
            return it;
        }

        constexpr ComponentPoolIterator& operator--() noexcept
        {
            offset_--;
            return *this;
        }

        constexpr ComponentPoolIterator operator--(int) noexcept
        {
            ComponentPoolIterator it = *this;
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

        [[nodiscard]] constexpr bool operator==(const ComponentPoolIterator& other) const noexcept
        {
            return offset_ == other.offset_;
        }

        [[nodiscard]] constexpr bool operator!=(const ComponentPoolIterator& other) const noexcept
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

    template<typename Component>
    class ComponentPool : public SparseSet
    {
    private:
        using PackedComponentsType = std::vector<Component>;

    public:
        using Iterator = ComponentPoolIterator<PackedComponentsType>;

    private:
    
    public:
        ComponentPool() = default;
        ~ComponentPool() = default;
    };
}