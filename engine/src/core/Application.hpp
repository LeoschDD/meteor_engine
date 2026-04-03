#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"

namespace meteor
{
    class Application
    {
    private:
        bool running_{false};


    public:
        Application();
        virtual ~Application();

        void Run();
    };
}