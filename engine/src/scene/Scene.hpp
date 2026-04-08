#pragma once

#include "ecs/World.hpp"
#include "core/Event.hpp"

namespace meteor
{
    class Scene
    {
    public:
        Scene();
        ~Scene() = default;

        void OnStart();
        void OnActivate();
        void OnDeactivate();

        void OnEvent(Event& event);
        void OnUpdate(const float dt);
        void OnRender();

    private:
        bool started_{false};
        ecs::World world_;
    };
}