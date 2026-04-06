#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"
#include "scene/Scene.hpp"

namespace meteor
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        bool running_{false};
        Scene scene_;
    };
}