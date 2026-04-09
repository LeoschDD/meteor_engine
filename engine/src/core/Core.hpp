#pragma once

#define METEOR_ENABLE_ASSERTS

#ifdef __linux__
    #define METEOR_PLATFORM_LINUX
#elifdef _WIN32
    #define METEOR_PLATFORM_WINDOWS
#endif

#ifdef METEOR_ENABLE_ASSERTS
    #ifdef METEOR_PLATFORM_WINDOWS
        #define METEOR_ASSERT(x, ...) {if(!(x)) {METEOR_ERROR("Assertion failed: {}", __VA_ARGS__); assert(false);}}
        #define METEOR_CORE_ASSERT(x, ...) {if(!(x)) {METEOR_CORE_ERROR("Assertion failed: {}", __VA_ARGS__); assert(false);}}
    #elifdef METEOR_PLATFORM_LINUX
        #define METEOR_ASSERT(x, ...) {if(!(x)) {METEOR_ERROR("Assertion failed: {}", __VA_ARGS__); assert(false);}}
        #define METEOR_CORE_ASSERT(x, ...) {if(!(x)) {METEOR_CORE_ERROR("Assertion failed: {}", __VA_ARGS__); assert(false);}}
    #else
        #define METEOR_ASSERT(x, ...)
        #define METEOR_CORE_ASSERT(x, ...)
    #endif
#else
    #define METEOR_ASSERT(x, ...)
    #define METEOR_CORE_ASSERT(x, ...)
#endif