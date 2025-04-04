cmake_minimum_required(VERSION 3.28.3)
project(RETRO2025 VERSION 0.0.1)

# Find required packages (ensure that the packages are installed and correctly linked)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Add the executable
add_executable(RETRO2025 Graphics/Shader.cpp  Main.cpp)


# Link the libraries
target_link_libraries(RETRO2025
    OpenGL::GL   # OpenGL core library
    GLEW::GLEW   # GLEW library
    glfw         # GLFW library
)


# Optionally, add include directories for the dependencies
target_include_directories(RETRO2025
    PRIVATE
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/scr/Graphics
)



# Set C++ standard (if necessary, e.g., C++11)
set_target_properties(RETRO2025 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)


