#include "core/Log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

meteor::Log::Log()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");

    core_logger_ = spdlog::stdout_color_mt("METEOR");
    core_logger_->set_level(spdlog::level::trace);

    client_logger_ = spdlog::stdout_color_mt("APP");
    client_logger_->set_level(spdlog::level::trace);
}