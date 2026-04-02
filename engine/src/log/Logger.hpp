#pragma once

#include "core/Headers.hpp"

namespace meteor
{
    class Logger
    {
    private:
        Logger();
        ~Logger();

    public:
        static Logger& Get()
        {
            static Logger instance;
            return instance;
        }
    };
}

#define METEOR_CORE_TRACE (...) meteor::Logger::Get()
#define METEOR_CORE_INFO (...)
#define METEOR_CORE_WARN (...)
#define METEOR_CORE_ERROR (...)

#define METEOR_TRACE (...)
#define METEOR_INFO (...)
#define METEOR_WARN (...)
#define METEOR_ERROR (...)
