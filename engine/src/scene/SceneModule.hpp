#pragma once

namespace meteor
{
    class SceneModule
    {
    protected:
        SceneModule() = default;

    public:
        virtual ~SceneModule() = default;

        virtual void OnStart() {};
        virtual void OnActivate() {};
        virtual void OnDeactivate() {};
        virtual void OnUpdate(const float dt) {};
    };
}