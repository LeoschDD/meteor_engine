cmake_minimum_required(VERSION 4.0)
project(meteor_engine LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/spdlog)
add_subdirectory(external/json)

include(cmake/imgui.cmake)

add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(sandbox)