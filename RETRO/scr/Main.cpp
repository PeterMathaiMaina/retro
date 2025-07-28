#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <windows.h>
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
#include "graphics/Model.hpp"
#include "core/CleanUp.h"
#include "assetManager/GlobalDeclarations.h"
#include "Headers/Callbacks.h"
#include <bitset>
#include "Shader/ShaderSetup.h"

const unsigned int WINDOW_WIDTH = 1800, WINDOW_HEIGHT = 1000;
const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
float aspect = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
float shd_aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
float lastFrame = 0.0f;
using hr_clock = std::chrono::high_resolution_clock;
auto lastFrameTime = hr_clock::now();
const double targetFPS = 60.0;
const double targetFrameDuration = 1.0 / targetFPS;
Camera camera(glm::vec3(0.0f, 0.3f, 1.2f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

namespace FlashLight {
    bool flashlightTogglePressed = false;
    bool flashlightOn = true;
}

struct CameraData {
    glm::mat4 view;
    glm::mat4 projection;
};

glm::vec3 lightPositions[] = {
    glm::vec3(20.40188f, 0.0f, 0.0f), glm::vec3(48.2f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 5.0f, 0.0f), glm::vec3(-2.0f, -1.0f, 0.0f), glm::vec3(3.0f, 0.5f, 0.0f),
    glm::vec3(-1.5f, 2.5f, 0.0f), glm::vec3(0.5f, -2.0f, -1.0f), glm::vec3(-20.0f, 9.0f, 0.5f),
    glm::vec3(-3.0f, 9.5f, -2.0f), glm::vec3(2.0f, 3.0f, 4.0f), glm::vec3(-0.5f, 1.5f, -3.0f),
    glm::vec3(1.5f, -1.0f, 2.0f), glm::vec3(-2.5f, 0.0f, -1.5f), glm::vec3(0.0f, -3.0f, 1.0f),
    glm::vec3(3.5f, 2.0f, -0.5f), glm::vec3(-1.0f, -2.5f, 0.0f), glm::vec3(2.5f, 0.8f, 3.0f),
    glm::vec3(-3.5f, 1.2f, -1.0f)
};

glm::mat4 GetRotationAroundYPoint(glm::vec3 pivotPoint, float radius, float speed, float scale) {
    float time = glfwGetTime();
    float angle = time * speed;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pivotPoint);
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(radius, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale));
    return model;
}

std::vector<glm::vec3> CalculateTangents(const float* vertices, size_t vertexCount) {
    std::vector<glm::vec3> tangents(vertexCount);
    for (size_t i = 0; i < vertexCount; i += 3) {
        glm::vec3 pos1(vertices[i * 8 + 0], vertices[i * 8 + 1], vertices[i * 8 + 2]);
        glm::vec2 uv1(vertices[i * 8 + 3], vertices[i * 8 + 4]);
        glm::vec3 pos2(vertices[(i + 1) * 8 + 0], vertices[(i + 1) * 8 + 1], vertices[(i + 1) * 8 + 2]);
        glm::vec2 uv2(vertices[(i + 1) * 8 + 3], vertices[(i + 1) * 8 + 4]);
        glm::vec3 pos3(vertices[(i + 2) * 8 + 0], vertices[(i + 2) * 8 + 1], vertices[(i + 2) * 8 + 2]);
        glm::vec2 uv3(vertices[(i + 2) * 8 + 3], vertices[(i + 2) * 8 + 4]);
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);
        tangents[i + 0] = tangent;
        tangents[i + 1] = tangent;
        tangents[i + 2] = tangent;
    }
    return tangents;
}


int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RETRO", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }
    float quadVertices[] = {
        -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 0.0f,
        1.0f, -1.0f,   1.0f, 0.0f,

        -1.0f,  1.0f,   0.0f, 1.0f,
        1.0f, -1.0f,   1.0f, 0.0f,
        1.0f,  1.0f,   1.0f, 1.0f
    };
    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0); 


    GLuint HDRfrb;
    glGenFramebuffers(1,&HDRfrb);
    glBindFramebuffer(GL_FRAMEBUFFER,HDRfrb);
    GLuint HDRtexture;
    glGenTextures(1,&HDRtexture);
    glBindTexture(GL_TEXTURE_2D,HDRtexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,WINDOW_WIDTH,WINDOW_HEIGHT,0,GL_RGBA,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,HDRtexture,0);

    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "HDR FRAMEBUFFER FAILED TO COMPLETE" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    GLuint HeightmapTexture = LoadHeightMapTexture("C:\\Users\\user\\retro\\RETRO\\rescources\\textures\\Uncompressed\\HEIGHTMAP.png");

    Camera* cameraPtr = &camera;
    setupcallbacks(window, cameraPtr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwWindowHint(GLFW_SAMPLES, 4);
    Model Bunny("C:\\Users\\user\\retro\\RETRO\\rescources\\models_raw\\bunny\\scene.gltf");
    Model House("C:\\Users\\user\\retro\\RETRO\\rescources\\models_raw\\House1\\House.obj");
    Model Tree("C:\\Users\\user\\retro\\RETRO\\rescources\\models_raw\\tree\\TreeLarge_0.obj");
    // Model CharacterModel("C:\\Users\\user\\retro\\RETRO\\rescources\\models_raw\\CharacterModel\\Man_test_4.obj");

    //GLint SpecularMap = TextureFromFile("Specular.jpe", "C:\\Users\\user\\retro\\RETRO\\rescources\\textures\\Uncompressed");
    //GLint DiffuseMap = TextureFromFile("bricks2.jpg", "C:\\Users\\user\\retro\\RETRO\\rescources\\textures\\Uncompressed");
    //GLint NormalMap = TextureFromFile("bricks2_normal.jpg", "C:\\Users\\user\\retro\\RETRO\\rescources\\textures\\Uncompressed");
    //GLint DisplacmentMap = TextureFromFile("bricks2_disp.jpg", "C:\\Users\\user\\retro\\RETRO\\rescources\\textures\\Uncompressed");
    HeightMap heightmap("C:\\Users\\user\\retro\\RETRO\\rescources\\textures\\Uncompressed\\HEIGHTMAP.png", 5.0);
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 2000.0f);
    Shader CubeShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_CUBE.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_CUBE.frag", nullptr);
    Shader LightCubeShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_LIGHTING_CUBES.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_LIGHTING_CUBES.frag", nullptr);
    Shader HouseShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HOUSE_SHADER.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HOUSE_SHADER.frag", nullptr);
    Shader HeightMapShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HEIGHTMAP.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HEIGHTMAP.frag", nullptr);
    Shader BunnyShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_BUNNY.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_BUNNY.frag", nullptr);
    Shader TreeShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_TREES.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_TREES.frag", nullptr);
    Shader HDRShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HDR.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HDR.frag", nullptr);
    BunnyShader.use();
    glm::mat4 BunnyMatrix = glm::mat4(1.0f);
    BunnyMatrix = glm::scale(BunnyMatrix, glm::vec3(0.7));
    BunnyMatrix = glm::translate(BunnyMatrix, glm::vec3(10.0f, 0.0f, 0.0f));
    BunnyShader.setMat4("model", BunnyMatrix);
    BunnyShader.setMat4("projection", projectionMatrix);
    BunnyShader.setvec3("aSpotlightPositon", lightPositions[10]);
    setSpotLight(BunnyShader, "spotlight", lightPositions[10], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.7f), glm::vec3(0.8f), glm::vec3(0.01f), 1.0f, 0.02f, 0.004f, glm::cos(glm::radians(10.5f)), glm::cos(glm::radians(12.5f)), FlashLight::flashlightOn);
    //CubeShader.use();
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, DiffuseMap);
    //CubeShader.setInt("texture_diffuse1", 0);
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, SpecularMap);
    //CubeShader.setInt("texture_specular1", 1);
    //glActiveTexture(GL_TEXTURE2);
    //glBindTexture(GL_TEXTURE_2D, NormalMap);
    //CubeShader.setInt("texture_normal1", 2);
    //glActiveTexture(GL_TEXTURE3);
    //glBindTexture(GL_TEXTURE_2D, DisplacmentMap);
    //CubeShader.setInt("texture_Displacement1", 3);
    //CubeShader.setvec3("aSpotlightPositon", lightPositions[10]);
    LightCubeShader.use();
    glm::mat4 LightCubeMatrix = glm::mat4(1.0f);
    LightCubeMatrix = glm::translate(LightCubeMatrix, lightPositions[10]);
    LightCubeShader.setMat4("model", LightCubeMatrix);
    LightCubeShader.setMat4("projection", projectionMatrix);
    TreeShader.use();
    glm::mat4 treeMat = glm::mat4(1.0f);
    treeMat = glm::translate(treeMat, lightPositions[0]);
    TreeShader.setMat4("model", treeMat);
    HouseShader.use();
    glm::mat4 houseMat = glm::mat4(1.0f);
    houseMat = glm::translate(houseMat, glm::vec3(48.2f, -4.40f, 0.0f));
    houseMat = glm::scale(houseMat, glm::vec3(1.3f));
    HouseShader.setMat4("model", houseMat);
    HouseShader.setMat4("projection", projectionMatrix);
    setPointLight(HouseShader, "pointlight", glm::vec3(0.5,0.5,0.5),lightPositions[0],glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);

    // setSpotLight(HouseShader, "spotlight",camera.Position,camera.Front, glm::vec3(2.7f), glm::vec3(0.8f), glm::vec3(0.01f), 1.0f, 0.02f, 0.004f, glm::cos(glm::radians(10.5f)), glm::cos(glm::radians(12.5f)), FlashLight::flashlightOn);
    Input input;
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    while (!glfwWindowShouldClose(window)) {
        auto startTime = hr_clock::now();
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glEnable(GL_MULTISAMPLE);
        input.processInput(window, deltaTime, camera, FlashLight::flashlightTogglePressed, FlashLight::flashlightOn);
        TreeShader.use();
        TreeShader.setMat4("projection", projectionMatrix);
        TreeShader.setMat4("view", cameraPtr->GetViewMatrix());
        BunnyShader.use();
        BunnyShader.setMat4("view", camera.GetViewMatrix());
        BunnyShader.setvec3("viewPos", cameraPtr->Position);
        CubeShader.setvec3("viewPos", cameraPtr->Position);
        LightCubeShader.setMat4("view", cameraPtr->GetViewMatrix());
        HouseShader.use();
        HouseShader.setMat4("view", cameraPtr->GetViewMatrix());
        HouseShader.setvec3("viewpos", cameraPtr->Position);

        glBindFramebuffer(GL_FRAMEBUFFER,HDRfrb);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        House.Draw(HouseShader);
        Tree.Draw(TreeShader);
        Bunny.Draw(BunnyShader);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        HeightMapShader.use();
        glActiveTexture(GL_TEXTURE0);  
        glBindTexture(GL_TEXTURE_2D, HeightmapTexture);
        HeightMapShader.setInt("terrain_texture", 0);
        heightmap.Draw(HeightMapShader, glm::mat4(1.0f), camera.GetViewMatrix(), projectionMatrix);


        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Disable depth testing for quad
        glDisable(GL_DEPTH_TEST);
        HDRShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, HDRtexture);
        HDRShader.setInt("hdrBuffer", 0);
        HDRShader.setFloat("exposure", 0.115f);  
        glBindVertexArray(quadVAO); 
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

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