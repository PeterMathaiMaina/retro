#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include "core/Shader.hpp"
#include "core/HeightMap.hpp"
#include "../third_party/imageprocessing/stb_image.h"
#include "../third_party/imageprocessing/gli/gli.hpp"
#include "../third_party/glm/glm.hpp"
#include "../third_party/glm/gtc/matrix_transform.hpp"
#include "../third_party/glm/gtc/type_ptr.hpp"
#include "Input/Input.h"
#include "Camera/Camera.hpp"
#include "graphics/Mesh.hpp"
#include "../textureLoader/textureLoader.hpp"
#include "graphics/Model.h"
#include "core/ShaderManager.hpp"
#include "AssetManager/AssetManager.h"
#include "Headers/Callbacks.h"
#include <bitset>
#include "Shader/ShaderSetup.h"
#include "Game/Game.h"
#include "BackEnd/BackEnd.h"
#include "Types/GlobalFunctions.h"
#include "CAMERA/Camera.h"


float lastFrame{};

namespace FlashLight {
    bool flashlightTogglePressed = false;
    bool flashlightOn = true;
}



int main() {
    Game Retro(GetscrWIDTH(),GetscrHEIGHT());
    Retro.Init();

    if (!BackEnd::InitGLFWWindow(GetscrWIDTH(), GetscrHEIGHT(), "RETRO")) {
        std::cerr << "Engine failed to start!\n";
        return -1;
    }
    ShaderManager::Init();
    Input::Init(BackEnd::GetWindow());

    while (BackEnd::WindowIsOpen() && !AssetManager::LoadingComplete()){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        AssetManager::UpdateLoading();
        Retro.RenderLoadingScreen(AssetManager::GetProgress());
        BackEnd::SwapBuffers();
        BackEnd::PollEvents();
    }

    while (BackEnd::WindowIsOpen()) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Retro.ProcessInput(deltaTime);
        Retro.Update(deltaTime);
        Retro.Render();

        BackEnd::SwapBuffers();
        BackEnd::PollEvents();
    }

    BackEnd::ShutDown();
    return 0;
}