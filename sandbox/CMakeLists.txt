add_library(sandbox_library)

file(GLOB_RECURSE
    SANDBOX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

list(REMOVE_ITEM 
    SANDBOX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

target_sources(sandbox_library
    PRIVATE
        ${SANDBOX_SRC}
)

target_include_directories(sandbox_library
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(sandbox_library 
    PUBLIC 
        meteor_engine
)

target_compile_definitions(sandbox_library
    PRIVATE
        SANDBOX_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

add_executable(sandbox src/main.cpp)

target_link_libraries(sandbox
    PRIVATE
        sandbox_library
)