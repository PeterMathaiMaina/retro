cmake_minimum_required(VERSION 3.28.3)
project(RETRO2025 VERSION 0.0.1)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Add executable and include stb_image.cpp
add_executable(RETRO2025 
    Graphics/Shader.cpp 
    Main.cpp 
    Utils/stb_image.cpp  
    Input/Input.cpp
)

# Link the required libraries
target_link_libraries(RETRO2025
    OpenGL::GL    
    GLEW::GLEW    
    glfw          
)

# Include directories (stb_image and other headers)
target_include_directories(RETRO2025 PRIVATE
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/Graphics
    ${CMAKE_SOURCE_DIR}/Utils
    ${CMAKE_SOURCE_DIR}/Input
)

# Set C++ standard
set_target_properties(RETRO2025 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)
