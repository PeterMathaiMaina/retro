#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include <iostream>
#include "../core/Shader.hpp"
#include "../../third_party/imageprocessing/stb_image.h"
#include "../../third_party/glm/glm.hpp"
#include "../../third_party/glm/gtc/matrix_transform.hpp"
#include "../../third_party/glm/gtc/type_ptr.hpp"
#include <iostream>

inline static bool spotlightKeyPressed = false;
inline static bool spotlightOn = false;

void Input::processInput(GLFWwindow* window, Shader& shader,
                         glm::vec3& cameraPos,  glm::vec3& cameraFront,  glm::vec3& cameraUp,float& deltaTime ,Camera& camera) {

    // Process input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS && !spotlightKeyPressed) {
        spotlightOn = !spotlightOn; // Toggle the spotlight
        spotlightKeyPressed = true;

        if (spotlightOn) {
            std::cout << "Spotlight is ON" << std::endl;
        } else {
            std::cout << "Spotlight is OFF" << std::endl;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_RELEASE) {
        spotlightKeyPressed = false;
    }
}

