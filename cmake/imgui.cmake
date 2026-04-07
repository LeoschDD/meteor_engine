if(TARGET imgui)
    return()
endif()

set(IMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/../external/imgui")

add_library(imgui STATIC)

target_sources(imgui 
    PRIVATE
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        "${IMGUI_DIR}/imgui_demo.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui 
    PUBLIC
        "${IMGUI_DIR}"
        "${IMGUI_DIR}/backends"
)

target_compile_features(imgui 
    PUBLIC 
        cxx_std_20
)