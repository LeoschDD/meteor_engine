add_library(meteor_engine STATIC)

file(GLOB_RECURSE 
    ENGINE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(meteor_engine 
    PRIVATE 
        ${ENGINE_SRC}
)

target_include_directories(meteor_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_precompile_headers(meteor_engine 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Headers.hpp
)

target_compile_features(meteor_engine 
    PUBLIC 
        cxx_std_20
)

target_link_libraries(meteor_engine
    PUBLIC
        glfw
        glad
        imgui
        spdlog
        nlohmann_json::nlohmann_json
)

target_compile_definitions(meteor_engine 
    PRIVATE
        ENGINE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)