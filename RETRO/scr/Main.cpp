#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include "core/Shader.hpp"
#include "../third_party/imageprocessing/stb_image.h"
#include "../third_party/imageprocessing/gli/gli.hpp"
#include "../third_party/glm/glm.hpp"
#include "../third_party/glm/gtc/matrix_transform.hpp"
#include "../third_party/glm/gtc/type_ptr.hpp"
#include "Input/Input.h"
#include "Camera/Camera.hpp"
#include "graphics/Mesh.hpp"
#include "../textureLoader/textureLoader.hpp"
#include "graphics/Model.hpp"
#include "core/CleanUp.hpp"
#include "Headers/Callbacks.h"
#include <bitset>




float lastFrame = 0.0f;
using hr_clock = std::chrono::high_resolution_clock;
auto lastFrameTime = hr_clock::now();
const double targetFPS = 90.0;
const double targetFrameDuration = 1.0 / targetFPS; // ~0.01667 seconds (16.67 ms)
Camera camera(glm::vec3(0.0f,0.3f, 1.2f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

namespace FlashLight
{
    bool flashlightTogglePressed = false;
    bool flashlightOn = false; 
}
struct CameraData {
    glm::mat4 view;
    glm::mat4 projection;
};
glm::vec3 lightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f),
    glm::vec3( 1.0f,  5.0f,  0.0f),
    glm::vec3(-2.0f, -1.0f,  3.0f),
    glm::vec3( 3.0f,  0.5f, -2.5f),
    glm::vec3(-1.5f,  2.5f,  1.0f),
    glm::vec3( 0.5f, -2.0f, -1.0f),
    glm::vec3( 4.0f,  1.0f,  0.5f),
    glm::vec3(-3.0f, -0.5f, -2.0f),
    glm::vec3( 2.0f,  3.0f,  4.0f),
    glm::vec3(-0.5f,  1.5f, -3.0f),
    glm::vec3( 1.5f, -1.0f,  2.0f),
    glm::vec3(-2.5f,  0.0f, -1.5f),
    glm::vec3( 0.0f, -3.0f,  1.0f),
    glm::vec3( 3.5f,  2.0f, -0.5f),
    glm::vec3(-1.0f, -2.5f,  0.0f),
    glm::vec3( 2.5f,  0.8f,  3.0f),
    glm::vec3(-3.5f,  1.2f, -1.0f)
};

    




int main(){
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(1000, 650, "RETRO", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }

    float cubeVertices[] = {
        // positions         // texCoords // normals
        // back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

        // front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

        // left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

        // right face
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        // bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

        // top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
    };


    unsigned int amount = 150;
    glm::mat4* modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // Seed random generator

    for (unsigned int i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);

        // Random position in world space (adjust range as needed)
        float x = (rand() %  200 - 100); // Range: -100 to +100
        float y = (rand() % 70 - 0);   // Range: -20 to +20
        float z = (rand() % 200 - 100); // Range: -100 to +100
        model = glm::translate(model, glm::vec3(x, y, z));

        // Optional: Random rotation
        float angle = static_cast<float>(rand() % 360);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.4f, 0.6f, 0.8f));

        // Optional: Random scale
        float scale = static_cast<float>((rand() % 40) / 10.0f + 2.8f);
        model = glm::scale(model, glm::vec3(scale));

        modelMatrices[i] = model;
    }


    unsigned int CUBEVAO,CUBEVBO;
    glGenVertexArrays(1,&CUBEVAO);
    glGenBuffers(1,&CUBEVBO);
    glBindVertexArray(CUBEVAO);
    glBindBuffer(GL_ARRAY_BUFFER,CUBEVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),&cubeVertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3* sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(5* sizeof(float)));

    unsigned int InstanceVBO; 
    glGenBuffers(1, &InstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    GLsizei vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, nullptr);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1); 
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);


    unsigned int LightCubeVAO,LightCubeVBO;
    glGenVertexArrays(1,&LightCubeVAO);
    glGenBuffers(1,&LightCubeVBO);
    glBindVertexArray(LightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER,LightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),&cubeVertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3* sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(5* sizeof(float)));
    glBindVertexArray(0);
    Camera* cameraPtr = &camera;
    setupcallbacks(window,cameraPtr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwWindowHint(GLFW_SAMPLES,4);

    Input input;
    Shader CubeShader("/home/mathai/retro/RETRO/rescources/Shaders/GL_CUBE.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_CUBE.frag",nullptr);
    Shader LightCubeShader("/home/mathai/retro/RETRO/rescources/Shaders/GL_LIGHTING_CUBES.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_LIGHTING_CUBES.frag",nullptr);
    GLint SpecularMap = TextureFromFile("Tiles012_4K-JPG_Roughness.dds","/home/mathai/retro/RETRO/rescources/textures/Compressed");
    GLint DiffuseMap = TextureFromFile("Tiles012_4K-JPG_Color.dds","/home/mathai/retro/RETRO/rescources/textures/Compressed");
    
    glm::mat4 projectionMatrix =glm::perspective(glm::radians(camera.Zoom), (float)955 / (float)560, 0.001f, 1000.0f);
    while (!glfwWindowShouldClose(window)) {
        auto startTime = hr_clock::now();
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        input.processInput(window,deltaTime,camera,FlashLight::flashlightTogglePressed,FlashLight::flashlightOn,CubeShader);
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_MULTISAMPLE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CubeShader.use();
        CubeShader.setvec3("dirlight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        CubeShader.setvec3("dirlight.ambient", glm::vec3(0.1));
        CubeShader.setvec3("dirlight.diffuse", glm::vec3(0.8f));
        CubeShader.setvec3("dirlight.specular", glm::vec3(0.001f));
        CubeShader.setvec3("spotlight.ambient", glm::vec3(0.1f));
        CubeShader.setvec3("spotlight.diffuse", glm::vec3(0.6f));
        CubeShader.setvec3("spotlight.specular", glm::vec3(1.0f));
        CubeShader.setFloat("spotlight.constant", 1.0f);
        CubeShader.setFloat("spotlight.linear", 0.09f);
        CubeShader.setFloat("spotlight.quadratic", 0.032f);
        CubeShader.setvec3("spotlight.position", camera.Position);
        CubeShader.setvec3("spotlight.direction", camera.Front);
        CubeShader.setFloat("spotlight.cutOff", glm::cos(glm::radians(10.5f)));
        CubeShader.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(20.5f)));
        CubeShader.setBool("spotlight.enabled", FlashLight::flashlightOn);
        CubeShader.setMat4("u_View", camera.GetViewMatrix());
        CubeShader.setvec3("u_ViewPos",camera.Position);
        CubeShader.setMat4("u_Projection", projectionMatrix); 
        CubeShader.setvec3("pointLights[0].position", lightPositions[0]);
        CubeShader.setvec3("pointLights[0].ambient", glm::vec3(1.5f));
        CubeShader.setvec3("pointLights[0].diffuse", glm::vec3(1.8f));
        CubeShader.setvec3("pointLights[0].specular", glm::vec3(1.5f));
        CubeShader.setFloat("pointLights[0].constant", 1.0f);
        CubeShader.setFloat("pointLights[0].linear", 0.09f);
        CubeShader.setFloat("pointLights[0].quadratic", 0.032f); 

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,DiffuseMap);
        CubeShader.setInt("texture_diffuse1",0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,SpecularMap);
        CubeShader.setInt("texture_specular1",1);
        bool SpecularEnabled = true;
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
           CubeShader.setBool("SpecularEnabled",SpecularEnabled);
        else 
            CubeShader.setBool("SpecularEnabled",false);
        glBindVertexArray(CUBEVAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, amount); 
        glBindVertexArray(0);
        glBindVertexArray(LightCubeVAO);
        LightCubeShader.use();
        glm::mat4 LightCubeMat = glm::mat4(1.0f);
        LightCubeMat = glm::translate(LightCubeMat,lightPositions[0]);
        LightCubeShader.setMat4("u_Model",LightCubeMat);
        LightCubeShader.setMat4("u_View",camera.GetViewMatrix());
        LightCubeShader.setMat4("u_Projection",projectionMatrix);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);

        //std::cout << "DRAWING THIS SHIET" << std::endl;
        // std::cout << "x: "<<cameraPtr->Position.x<< "y: "<<cameraPtr->Position.y<< "z: "<<cameraPtr->Position.z <<std::endl;
        auto endTime = hr_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;
        double frameTime = elapsed.count();        
        if (frameTime < targetFrameDuration) {
            std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameDuration - frameTime));
        }
        lastFrame = currentFrame;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


