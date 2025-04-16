#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Graphics/Shader.hpp"
#include "../Utils/stb_image.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../Input/Input.hpp"
#include <iostream>


void Input::processInput(GLFWwindow* window, Shader& shader,
                         glm::vec3& cameraPos,  glm::vec3& cameraFront,  glm::vec3& cameraUp,float& deltaTime ,Camera& camera) {

    float cameraSpeed = 15.0f *deltaTime; 

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

