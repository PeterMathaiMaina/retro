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
#include "core/CleanUp.h"
#include "core/ShaderManager.hpp"
#include "core/GlobalDef.h"
#include "AssetManager/AssetManager.h"
#include "Headers/Callbacks.h"
#include <bitset>
#include "Shader/ShaderSetup.h"
#include "Game/Game.h"
#include "BackEnd/BackEnd.h"
#include "Types/Retro_types.h"


Camera camera(glm::vec3(0.0f, 0.3f, 1.2f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
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
    while (BackEnd::WindowIsOpen() && !AssetManager::LoadingComplete()){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        AssetManager::UpdateLoading();
        Retro.RenderLoadingScreen(AssetManager::GetProgress());
        BackEnd::SwapBuffers();
        BackEnd::PollEvents();
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout<<AssetManager::AssetsToLoad.size()<<std::endl;


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