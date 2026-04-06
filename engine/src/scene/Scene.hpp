#pragma once

#include "ecs/World.hpp"

namespace meteor
{
    class Scene
    {
    private:
        void InitWorld();    

    public:
        Scene();
        ~Scene() = default;

        void OnUpdate();

    private:
        ecs::World world_;
    };
}