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

void Input::processInput(GLFWwindow* window,float& deltaTime ,Camera& camera,bool &flashlightTogglePressed,bool &flashlightOn) {
    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
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
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !flashlightTogglePressed) {
        flashlightTogglePressed = true;
        flashlightOn = !flashlightOn;  
        // std::cout << "Flashlight : " << (flashlightOn ? "ON" : "OFF") << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
        //std::cout << "FUCKED TWIT!!!!"<<std::endl;
        //std::cout << "FUCKED TWIT!!!!"<<std::endl;
        //std::cout << "FUCKED TWIT!!!!"<<std::endl;
        //std::cout << "FUCKED TWIT!!!!"<<std::endl;
        //std::cout << "FUCKED TWIT!!!!"<<std::endl;
        //std::cout << "FUCKED TWIT!!!!"<<std::endl;
        //std::cout << "FUCKED TWIT!!!!"<<std::endl;
        //std::cout << "FUCKED TWIT!!!!"<<std::endl;
        //std::cout << "FUCKED TWIT!!!!"<<std::endl;
        flashlightTogglePressed = false;
    }

}

