#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
// #include "../Camera/Camera.hpp"
// #include "../CAMERA/Camera.h

struct BackEnd{
    static bool InitGLFWWindow(int width,int height,const std::string& title);
    static void ShutDown();
    static bool WindowIsOpen();
    static void PollEvents();
    static void SwapBuffers();


    static GLFWwindow* GetWindow();

    // static Camera* GetCamera();


private:
    static GLFWwindow* window;
};