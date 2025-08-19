#include "BackEnd.h"
#include <iostream>

GLFWwindow* BackEnd::window = nullptr;

bool BackEnd::InitGLFWWindow(int width, int height, const std::string& title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!\n";
        return false;
    }

    // Set GLFW options (OpenGL version, profile, etc.)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!\n";
        glfwTerminate();
        return false;
    }

    // Make context current
    glfwMakeContextCurrent(window);

    // Init GLEW (after making context current)
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!\n";
        return false;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) << "\n";

    glEnable(GL_DEPTH_TEST);

    return true;
}

void BackEnd::ShutDown() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool BackEnd::WindowIsOpen() {
    return window && !glfwWindowShouldClose(window);
}

void BackEnd::PollEvents() {
    glfwPollEvents();
}

void BackEnd::SwapBuffers() {
    glfwSwapBuffers(window);
}

GLFWwindow* BackEnd::GetWindow() {
    return window;
}
