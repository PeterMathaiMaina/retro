#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "core/Shader.hpp"
#include "../third_party/imageprocessing/stb_image.h"
#include "../third_party/imageprocessing/gli/gli.hpp"
#include "../third_party/glm/glm.hpp"
#include "../third_party/glm/gtc/matrix_transform.hpp"
#include "../third_party/glm/gtc/type_ptr.hpp"
#include "Input/Input.h"
#include "Camera/Camera.hpp"
#include "graphics/Mesh.hpp"
#include "../textureLoader/textureLoader.hpp"
#include "graphics/Model.hpp"

float lastFrame = 0.0f;


using hr_clock = std::chrono::high_resolution_clock;

auto lastFrameTime = hr_clock::now();

const double targetFPS = 60.0;
const double targetFrameDuration = 1.0 / targetFPS; // ~0.01667 seconds (16.67 ms)

Camera camera(glm::vec3(0.0f,0.3f, 1.2f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

float lastX = 960.0f / 2.0f;
float lastY = 560.0f / 2.0f;

bool firstMouse = true;

float fov = 55.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main(){
    // GLFW initialization
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(950, 600, "RETRO", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }
    const GLubyte* version = glGetString(GL_VERSION);

    // Setup input callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input input;

    while (!glfwWindowShouldClose(window)) {
        auto startTime = hr_clock::now();


        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);


        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        input.processInput(window,camera.Position, camera.Front, camera.Up, deltaTime, camera);   

        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        auto endTime = hr_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;
        double frameTime = elapsed.count(); 






        if (frameTime < targetFrameDuration) {
            std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameDuration - frameTime));
        }


        lastFrame = currentFrame;
        if (deltaTime < targetFrameDuration) {
            std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameDuration - deltaTime));
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 90.0f)
        fov = 90.0f;
}
