cmake_minimum_required(VERSION 3.20)
project(Ariadnis C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)

FetchContent_MakeAvailable(glfw glm)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES} "src/ui/navigation/top_bar.h")

target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(imgui PUBLIC 
    glfw 
    opengl32
)

# --- Main Executable ---
add_executable(Ariadnis 
    src/main.cpp 
    src/glad.c
    src/core/camera/camera.h
    src/core/terrain/terrain.cpp
    src/core/terrain/terrain.h
    src/core/camera/camera.cpp
    src/core/render/render-modes.h

    # --- UI SOURCES ---
    src/ui/navigation/top_bar.h)

target_include_directories(Ariadnis PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Ariadnis PRIVATE 
    glfw 
    glm::glm
    opengl32
    imgui
)