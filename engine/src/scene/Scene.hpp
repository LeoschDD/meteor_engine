#pragma once

#include "ecs/World.hpp"
#include "scene/SceneModule.hpp"
#include "core/Event.hpp"

namespace meteor
{
    class Scene
    {
    private:
        using ModuleStack = std::vector<std::unique_ptr<SceneModule>>;

    public:
        Scene();
        ~Scene() = default;

        void OnStart();
        void OnActivate();
        void OnDeactivate();

        void OnEvent(Event& event);
        void OnUpdate(const float dt);

        [[nodiscard]] bool Started() const noexcept
        {
            return started_;
        }

    private:
        bool started_{false};

        ecs::World world_;
        ModuleStack modules_;
    };
}