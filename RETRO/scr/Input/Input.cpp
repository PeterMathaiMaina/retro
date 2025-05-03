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


void Input::processInput(GLFWwindow* window, Shader& shader,
                         glm::vec3& cameraPos,  glm::vec3& cameraFront,  glm::vec3& cameraUp,float& deltaTime ,Camera& camera) {

    float cameraSpeed = 20.2f *deltaTime; 

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
    

}

