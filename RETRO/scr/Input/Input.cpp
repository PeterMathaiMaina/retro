#include "Input.h"         // Your class declaration
#include <iostream>          // For std::cout, std::endl
#include <GLFW/glfw3.h>      // For glfwGetKey, GLFW_KEY_*, etc.


void Input::processInput(GLFWwindow* window, Shader& shader, float& mixture) const
{
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK ) == GLFW_PRESS)
    {    
        std::cout<<"WINDOW CLOSED"<<std::endl;
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixture += 0.05f; 
        std::cout<<"UP KEY PRESSED"<<std::endl;
        if (mixture > 1.0f) mixture = 1.0f; 
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixture -= 0.05f; 
        std::cout<<"DOWN KEY PRESSED"<<std::endl;
        if (mixture < 0.0f) mixture = 0.0f;         
    }
}
