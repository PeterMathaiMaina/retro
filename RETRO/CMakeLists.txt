cmake_minimum_required(VERSION 3.28.3)
project(RETRO2025 )

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)


# Add executable and include stb_image.cpp
add_executable(RETRO2025 
    scr/Main.cpp 
    scr/core/Shader.cpp 
    third_party/imageprocessing/stb_image.cpp  
    scr/Input/Input.cpp
    scr/Camera/Camera.hpp
    scr/graphics/Mesh.hpp
    scr/graphics/Model.hpp
    textureLoader/textureLoader.cpp
)

# Link the required libraries
target_link_libraries(RETRO2025
    OpenGL::GL    
    GLEW::GLEW    
    glfw      
    assimp    
)

# Include directories (stb_image and other headers)
target_include_directories(RETRO2025 PRIVATE
    ${CMAKE_SOURCE_DIR}/scr
    ${CMAKE_SOURCE_DIR}/scr/core
    ${CMAKE_SOURCE_DIR}/scr/graphics
    ${CMAKE_SOURCE_DIR}/scr/Input
    ${CMAKE_SOURCE_DIR}/scr/Camera
    ${CMAKE_SOURCE_DIR}/textureLoader
    ${CMAKE_SOURCE_DIR}/third_party
    ${CMAKE_SOURCE_DIR}/third_party/glm
)


# Set C++ standard
set_target_properties(RETRO2025 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

