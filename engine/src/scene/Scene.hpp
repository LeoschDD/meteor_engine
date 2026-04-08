#pragma once

#include "ecs/World.hpp"
#include "core/Event.hpp"

namespace meteor
{
    class Scene
    {
    public:
        explicit Scene(const std::string& name);
        Scene(const std::string& name, ecs::World world);

        ~Scene() = default;

        void OnStart();
        void OnActivate();
        void OnDeactivate();

        void OnEvent(Event& event);
        void OnUpdate(const float dt);
        void OnRender();
        
        [[nodiscard]] const std::string& GetName() const noexcept {return name_;}
        [[nodiscard]] ecs::World& GetWorld() noexcept {return world_;}

    private:
        bool started_{false};
        std::string name_;
        ecs::World world_;
    };
}