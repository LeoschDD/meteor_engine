#pragma once

#include "ecs/Ecs.hpp"
#include "core/Event.hpp"

namespace meteor
{
    class SystemBase
    {
    public:
        SystemBase() = default;
        virtual ~SystemBase() = default;

        virtual void OnCreate(ecs::World& world) {}
        virtual void OnDestroy(ecs::World& world) {}

        virtual void OnActivate(ecs::World& world) {}
        virtual void OnDeactivate(ecs::World& world) {}

        virtual void OnEvent(ecs::World& world, Event& event) {}
        virtual void OnUpdate(ecs::World& world, const float dt) {}
    };
}