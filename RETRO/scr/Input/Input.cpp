#include "Input.h"
#include <iostream>

void Input::Init(GLFWwindow* window) {
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    std::cout << "CALLBACKS ARE LOCKING IN" << std::endl;
}
void Input::ProcessKeyboard(Camera& camera, float deltaTime) {
    float speed = 5.0f * deltaTime;

    if (glfwGetKey(BackEnd::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
        camera.SetPosition(camera.GetPosition() + camera.GetForward() * speed);

    if (glfwGetKey(BackEnd::GetWindow(),GLFW_KEY_S) == GLFW_PRESS)
        camera.SetPosition(camera.GetPosition() - camera.GetForward() * speed);

    if (glfwGetKey(BackEnd::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
        camera.SetPosition(camera.GetPosition() - camera.GetRight() * speed);

    if (glfwGetKey(BackEnd::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
        camera.SetPosition(camera.GetPosition() + camera.GetRight() * speed);

    if (glfwGetKey(BackEnd::GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.SetPosition(camera.GetPosition() + camera.GetUp() * speed);

    if (glfwGetKey(BackEnd::GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.SetPosition(camera.GetPosition() - camera.GetUp() * speed);
}


double Input::GetMouseOffsetX() {
    double x = mouseOffsetX;
    mouseOffsetX = 0.0; // reset after read
    return x;
}

double Input::GetMouseOffsetY() {
    double y = mouseOffsetY;
    mouseOffsetY = 0.0; // reset after read
    return y;
}

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    mouseOffsetX = xpos - lastX;
    mouseOffsetY = lastY - ypos; // inverted y (OpenGL coords)
    lastX = xpos;
    lastY = ypos;
}
