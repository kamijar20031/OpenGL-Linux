cmake_minimum_required(VERSION 3.10)
project(OpenGL_Demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1 
)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v5.3.1 
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.0 
)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v0.1.36
)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw glm assimp imgui glad stb)

find_package(OpenGL REQUIRED)


file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS src/**/*.cpp src/**/.c)
add_executable(OpenGL_Demo ${SRCS})

target_include_directories(OpenGL_Demo PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/main
    ${CMAKE_SOURCE_DIR}/src/objects
    ${CMAKE_SOURCE_DIR}/src/opengl
    ${CMAKE_SOURCE_DIR}/src/physics
    ${CMAKE_SOURCE_DIR}/src/helpers
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${glfw_SOURCE_DIR}/include
    ${stb_SOURCE_DIR}
)
add_library(imgui_lib
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_lib PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_lib PRIVATE glfw)
target_link_libraries(OpenGL_Demo PRIVATE imgui_lib glm assimp glad)

add_custom_command(TARGET OpenGL_Demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:OpenGL_Demo>"
        "${CMAKE_SOURCE_DIR}/src/OpenGL_Demo${CMAKE_EXECUTABLE_SUFFIX}"
    VERBATIM
)

if (WIN32)
    target_link_libraries(OpenGL_Demo PRIVATE opengl32 gdi32 user32 shell32)
else()
    target_link_libraries(OpenGL_Demo PRIVATE OpenGL::GL)
endif()
