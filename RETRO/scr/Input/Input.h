#ifndef INPUT_HPP
#define INPUT_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../third_party/glm/glm.hpp"
#include "../CAMERA/Camera.h"
#include "../BackEnd/BackEnd.h"

// Forward declare Camera if needed
struct Camera;

struct Input {
public:
    static void Init(GLFWwindow* window);

    // Mouse offset (delta movement)
    static double GetMouseOffsetX();
    static double GetMouseOffsetY();
    static void ProcessKeyboard(Camera& camera,float deltaTime);

    // Called internally by GLFW
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

private:
    static inline double lastX = 0.0;
    static inline double lastY = 0.0;
    static inline bool firstMouse = true;

    static inline double mouseOffsetX = 0.0;
    static inline double mouseOffsetY = 0.0;

};

#endif
