cmake_minimum_required(VERSION 3.15)
project(RETRO2025)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Set Assimp install folder here (make sure this is where assimpConfig.cmake is)
set(CMAKE_PREFIX_PATH "C:/msys64/mingw64" ${CMAKE_PREFIX_PATH})
# GLFW via FetchContent
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

add_definitions(-DGLEW_STATIC)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

add_executable(RETRO2025
    scr/Main.cpp
    scr/Renderer/Renderer.cpp
    scr/assetManager/GlobalDeclarations.cpp
    scr/assetManager/InputState.h
    scr/core/Shader.cpp
    third_party/imageprocessing/stb_image.cpp
    scr/Input/Input.cpp
    scr/Headers/callbacks.cpp
    textureLoader/textureLoader.cpp
    scr/Shader/ShaderSetup.cpp

)

target_include_directories(RETRO2025 PRIVATE
    ${CMAKE_SOURCE_DIR}/scr
    ${CMAKE_SOURCE_DIR}/scr/assetManager
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
    ${CMAKE_SOURCE_DIR}/Libs/glew-2.1.0/lib/Release/x64/glew32s.lib
    glfw
    OpenGL::GL
    assimp::assimp
)
