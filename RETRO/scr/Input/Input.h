#ifndef INPUT_H
#define INPUT_H

class Shader;
struct GLFWwindow;

struct Input
{
    void processInput(GLFWwindow* window, Shader& shader, float& mixture) const;
};

#endif
