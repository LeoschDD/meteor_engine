#pragma once

#include "ecs/ComponentPool.hpp"

namespace meteor::ecs
{
    using namespace internal;

    template<size_t Include, size_t Exclude>
    class ViewIterator
    {
    private:
        using IteratorType = SparseSet::Iterator;
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
            for (auto& pool : excluded_pools_)
            {
                if (pool->Contains(entity)) return false;
            }
            for (auto& pool : included_pools_)
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
        ViewIterator(SparseSet::Iterator current, 
            SparseSet::Iterator end, 
            std::array<const SparseSet*, Include> included, 
            std::array<const SparseSet*, Exclude> excluded)
            : current_(current)
            , end_(end)
            , pools_(pools)
            , pool_count_(pool_count)
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
            return current_== other.current_;
        }

    private:
        IteratorType current_;
        IteratorType end_;
        std::array<const SparseSet*, Include> included_pools_;
        std::array<const SparseSet*, Exclude> excluded_pools_;
    };

    template<typename... Components>
    requires(sizeof...(Components) != 0)
    class View
    {
    private:
        using PoolContainer = std::array<SparseSet*, sizeof...(Components)>;

    public:
        using Iterator = ViewIterator<sizeof...(Components), 0>;    

    private:
        void FindSmallestPool()
        {
            SparseSet* smallest_pool_ = pools_.front();
            for (auto& pool : pools_)
            {
                if (pool->Size() < smallest->Size())
                {
                    smallest_pool_ = pool;
                }
            }
        }

    public:
        explicit View(ComponentPool<Components>*... components) 
            : pools_(components...) 
        {
            FindSmallestPool();
        }

        ~View() = default;

        template<typename... Fn>
        void Each(Fn&&... fn)
        {
            [this, &fn]<size_t... Index>(std::index_sequence<Index...>){
                for (auto& entity : *this)
                {   
                    fn(entity, static_cast<ComponentPool<Components>*>(pools_[I])->Get(entity)...);
                }
            }(std::index_sequence_for<Components...>{});
        }

        [[nodiscard]] Iterator begin() const noexcept
        {
            return Iterator(
                smallest_pool_->begin(), 
                smallest_pool_->end(), 
                pools_, 
                std::array<SparseSet*, 0>());
        }
        [[nodiscard]] Iterator end() const noexcept
        {
            
            return Iterator(
                smallest_pool_->end(), 
                smallest_pool_->end(), 
                pools_, 
                std::array<SparseSet*, 0>());
        }

    private:
        PoolContainer pools_;
        SparseSet* smallest_pool_;
    };
}