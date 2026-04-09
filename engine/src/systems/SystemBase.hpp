#pragma once

#include "ecs/Ecs.hpp"

namespace meteor
{
    class SystemBase
    {
    public:
        SystemBase(ecs::World* world)
            : world_(world)
        {}     
        
        virtual ~SystemBase() = default;

        virtual void OnStart() {}
        virtual void OnUpdate() {}

    protected:
        ecs::World* world_;
    };
}