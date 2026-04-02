add_executable(editor)

file(GLOB_RECURSE
    EDITOR_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(editor
    PRIVATE
        ${EDITOR_SRC}
)

target_include_directories(editor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(editor
    PRIVATE
        meteor_engine
        sandbox_library
)

target_compile_definitions(editor 
    PRIVATE
        EDITOR_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)