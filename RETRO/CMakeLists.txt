cmake_minimum_required(VERSION 3.15)


# Optional: Force Clang (better to set env variables instead)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Enable ccache if found
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

project(RETRO2025)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)



# Optional: disable Wayland for GLFW if needed
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)

# GLFW via FetchContent
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# GLEW and Assimp
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

add_executable(RETRO2025
    scr/AssetManager/AssetManager.cpp
    scr/GlobalAssets/GlobalAssets.cpp
    scr/BackEnd/BackEnd.cpp
    scr/Main.cpp
    scr/Util/Util.cpp
    scr/Game/Game.cpp
    scr/Renderer/Renderer.cpp
    scr/core/Shader.cpp
    scr/core/ShaderManager.hpp
    scr/core/ShaderManager.cpp
    third_party/imageprocessing/stb_image.cpp
    scr/Input/Input.cpp
    scr/Headers/callbacks.cpp
    textureLoader/textureLoader.cpp
    scr/Shader/ShaderSetup.cpp
    scr/graphics/Model.cpp
)


target_include_directories(RETRO2025 PRIVATE
    ${CMAKE_SOURCE_DIR}/scr
    ${CMAKE_SOURCE_DIR}/scr/Util
    ${CMAKE_SOURCE_DIR}/scr/Types
    ${CMAKE_SOURCE_DIR}/scr/BackEnd
    ${CMAKE_SOURCE_DIR}/scr/Game
    ${CMAKE_SOURCE_DIR}/scr/AssetManager
    ${CMAKE_SOURCE_DIR}/scr/GlobalAssets/
    ${CMAKE_SOURCE_DIR}/scr/Renderer
    ${CMAKE_SOURCE_DIR}/scr/Shader
    ${CMAKE_SOURCE_DIR}/scr/core
    ${CMAKE_SOURCE_DIR}/scr/graphics
    ${CMAKE_SOURCE_DIR}/scr/Input
    ${CMAKE_SOURCE_DIR}/scr/Camera
    ${CMAKE_SOURCE_DIR}/textureLoader
    ${CMAKE_SOURCE_DIR}/third_party
    ${CMAKE_SOURCE_DIR}/third_party/glm
    ${CMAKE_SOURCE_DIR}/third_party/imageprocessing
    ${CMAKE_SOURCE_DIR}/third_party/imageprocessing/gli
    ${CMAKE_SOURCE_DIR}/scr/Headers
    ${CMAKE_SOURCE_DIR}/Libs/glew-2.1.0/include
)

target_link_libraries(RETRO2025
    glfw
    OpenGL::GL
    GLEW::GLEW
    assimp::assimp
)
