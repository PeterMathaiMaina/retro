#ifndef INPUT_HPP
#define INPUT_HPP

#include <GLFW/glfw3.h>
#include "../core/Shader.hpp"
#include "../../third_party/glm/glm.hpp"
#include "../Camera/Camera.hpp"

class Input {
public:
    void processInput(GLFWwindow* window, float& deltaTime,Camera& camera,bool &flashlightTogglePressed,bool &flashlightOn);
};

#endif
