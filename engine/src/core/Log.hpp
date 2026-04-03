#pragma once

#include "core/Headers.hpp"

namespace meteor
{
    class Log
    {
    private:
        std::shared_ptr<spdlog::logger> client_logger_;
        std::shared_ptr<spdlog::logger> core_logger_;

    private:
        Log();

    public:
        static Log& Get()
        {
            static Log instance;
            return instance;
        }

        std::shared_ptr<spdlog::logger> GetClientLogger() {return client_logger_;}
        std::shared_ptr<spdlog::logger> GetCoreLogger() {return core_logger_;}
    };
}

#define METEOR_CORE_TRACE(...) meteor::Log::Get().GetCoreLogger()->trace(__VA_ARGS__)
#define METEOR_CORE_INFO(...)  meteor::Log::Get().GetCoreLogger()->info(__VA_ARGS__)
#define METEOR_CORE_WARN(...)  meteor::Log::Get().GetCoreLogger()->warn(__VA_ARGS__)
#define METEOR_CORE_ERROR(...) meteor::Log::Get().GetCoreLogger()->error(__VA_ARGS__)

#define METEOR_TRACE(...) meteor::Log::Get().GetClientLogger()->trace(__VA_ARGS__)
#define METEOR_INFO(...)  meteor::Log::Get().GetClientLogger()->info(__VA_ARGS__)
#define METEOR_WARN(...)  meteor::Log::Get().GetClientLogger()->warn(__VA_ARGS__)
#define METEOR_ERROR(...) meteor::Log::Get().GetClientLogger()->error(__VA_ARGS__)
