#pragma once

#include "ecs/ComponentPool.hpp"
#include <array>

namespace meteor::ecs
{
    using namespace internal;

    class Registry;

    template<typename... Components>
    requires(sizeof...(Components) != 0)
    class View
    {
    private:
        using PoolContainer = std::array<std::unique_ptr<SparseSet>, sizeof...(Components)>;

    public:
        
    private:
        SparseSet* GetSmallestPool()
        {
            SparseSet* smallest = pools_.front()->get();
            for (auto& pool : pools_)
            {
                if (pool->Size() < smallest->Size())
                {
                    smallest = pool->get();
                }
            }
            return smallest;
        }

    public:
        explicit View(Components&... components) 
            : pools_(&components...) {}

        ~View() = default;

        template<typename... Fn>
        void Each(Fn&&... fn)
        {
            for (auto& entity : GetSmallestPool())
            {   
                entity
            }
        }

    private:
        PoolContainer pools_;
        Registry* registry_;
    };
}