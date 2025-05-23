#ifndef INPUT_HPP
#define INPUT_HPP

#include <GLFW/glfw3.h>
#include "../core/Shader.hpp"
#include "../../third_party/glm/glm.hpp"
#include "../Camera/Camera.hpp"

class Input {
public:
    void processInput(GLFWwindow* window, Shader& shader,
                      glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp,float& deltaTime,Camera& camera);
};

#endif
