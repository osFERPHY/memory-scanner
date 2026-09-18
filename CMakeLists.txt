cmake_minimum_required(VERSION 3.16)
project(memory_scanner)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_DIR "imgui")
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
        ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp)

set(PROJECT_SOURCES
        crs/main.cpp
        crs/Core/Application.cpp
        crs/UI/UI.cpp
        crs/Memory/process.cpp)


add_executable(memory_scanner ${PROJECT_SOURCES} ${IMGUI_SOURCES})

target_include_directories(memory_scanner PRIVATE
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        crs)

target_link_options(memory_scanner PRIVATE -static-libgcc -static-libstdc++)

target_link_libraries(memory_scanner PRIVATE
        d3d11 d3dcompiler
        ws2_32 comdlg32 winmm dwmapi)