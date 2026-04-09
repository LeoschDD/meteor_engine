#pragma once

#include "ecs/World.hpp"
#include "core/Event.hpp"
#include "systems/SystemBase.hpp"

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

        void CreateEntity();
        
        [[nodiscard]] const std::string& GetName() const noexcept {return name_;}
        [[nodiscard]] ecs::World& GetWorld() noexcept {return world_;}

    private:
        bool started_{false};
        std::string name_;

        ecs::World world_;
        std::vector<std::unique_ptr<SystemBase>> systems_;
    };
}