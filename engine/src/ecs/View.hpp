#pragma once

#include "ecs/ComponentPool.hpp"

namespace meteor::ecs
{
    template<size_t ComponentCount>
    class ViewIterator
    {
    private:
        using IteratorType = internal::SparseSet::Iterator;
        using IteratorTraits = std::iterator_traits<IteratorType>;

    public:
        using value_type = IteratorTraits::value_type;
        using pointer = IteratorTraits::pointer;
        using reference = IteratorTraits::reference;
        using difference_type = IteratorTraits::difference_type;

    private:
        [[nodiscard]] bool Valid() const 
        {
            Entity entity = *current_;
            for (auto& pool : pools_)
            {
                if (!pool->Contains(entity)) return false;
            }
            return true;
        }

        void StepToNext()
        {
            while (current_ != end_)
            {
                if (Valid()) break;
                ++current_;
            }
        }

    public:
        ViewIterator(IteratorType current, 
            IteratorType end, 
            std::array<internal::SparseSet*, ComponentCount> pools) 
            : current_(current)
            , end_(end)
            , pools_(pools)
        {
            StepToNext();
        }

        ViewIterator& operator++() noexcept
        {
            ++current_;
            StepToNext();
            return *this;
        }

        ViewIterator operator++(int) noexcept
        {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] pointer operator->() const noexcept 
        {
            return &*current_;
        }

        [[nodiscard]] reference operator*() const noexcept 
        {
            return *current_;
        }

        [[nodiscard]] constexpr bool operator==(const ViewIterator& other) const noexcept 
        {
            return current_ == other.current_;
        }

    private:
        IteratorType current_;
        IteratorType end_;
        std::array<internal::SparseSet*, ComponentCount> pools_;
    };

    template<typename... Components>
    requires(sizeof...(Components) != 0)
    class View
    {
    private:
        static constexpr size_t component_count  = sizeof...(Components);

        using PoolArray = std::array<internal::SparseSet*, component_count>;

    public:
        using Iterator = ViewIterator<component_count>;    

    private:
        void FindSmallestPool()
        {
            smallest_pool_ = pools_.front();
            for (auto& pool : pools_)
            {
                if (pool->Size() < smallest_pool_->Size())
                {
                    smallest_pool_ = pool;
                }
            }
        }

    public:
        explicit View(internal::ComponentPool<Components>*... pools) 
            : pools_{pools...} 
        {
            FindSmallestPool();
        }

        ~View() = default;

        template<typename Fn>
        void Each(Fn&& fn)
        {
            [this, &fn]<size_t... Index>(std::index_sequence<Index...>)
            {
                for (auto& entity : *this)
                {   
                    fn(entity, static_cast<internal::ComponentPool<Components>*>(pools_[Index])->Get(entity)...);
                }
            }(std::index_sequence_for<Components...>{});
        }

        [[nodiscard]] Iterator begin() const noexcept
        {
            return Iterator(smallest_pool_->begin(), smallest_pool_->end(), pools_);
        }
        [[nodiscard]] Iterator end() const noexcept
        {
            return Iterator(smallest_pool_->end(), smallest_pool_->end(), pools_);
        }

    private:
        PoolArray pools_;
        internal::SparseSet* smallest_pool_;
    };
}