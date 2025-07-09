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
#include "graphics/Model.hpp"
#include "core/CleanUp.hpp"
#include "Headers/Callbacks.h"
#include <bitset>
#include "Shader/ShaderSetup.h"



const unsigned int WINDOW_WIDTH = 1900,WINDOW_HEIGHT = 1080;
const unsigned int SHADOW_WIDTH = 4096,SHADOW_HEIGHT = 4096;
float shd_aspect  = (float)SHADOW_WIDTH/(float)SHADOW_HEIGHT;

float lastFrame = 0.0f;
using hr_clock = std::chrono::high_resolution_clock;
auto lastFrameTime = hr_clock::now();
const double targetFPS = 60.0;
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
    glm::vec3(20.40188f, 0.0f, -0.3057f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f),
    glm::vec3( 1.0f,  5.0f,  0.0f),
    glm::vec3(-2.0f, -1.0f,  3.0f),
    glm::vec3( 3.0f,  0.5f, -2.5f),
    glm::vec3(-1.5f,  2.5f,  1.0f),
    glm::vec3( 0.5f, -2.0f, -1.0f),
    glm::vec3( -20.0f,  9.0f,  0.5f),//10
    glm::vec3(-3.0f, 9.5f, -2.0f),//10000000
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
glm::mat4 GetRotationAroundYPoint(glm::vec3 pivotPoint, float radius, float speed, float scale) {
    float time = glfwGetTime();
    float angle = time * speed;

    glm::mat4 model = glm::mat4(1.0f);

    // 1. Move to the pivot point
    model = glm::translate(model, pivotPoint);

    // 2. Rotate around the pivot's Y axis
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    // 3. Move away from the pivot (orbit radius along X or Z)
    model = glm::translate(model, glm::vec3(radius, 0.0f, 0.0f));

    // 4. Scale the object
    model = glm::scale(model, glm::vec3(scale));

    return model;
}




int main(){
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);  // Use 24 or 32 bits
    glfwWindowHint(GLFW_SAMPLES,4);


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

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
    };


    unsigned int amount = 300;
    glm::mat4* modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // Seed random generator

    for (unsigned int i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);

        // Random position in world space (adjust range as needed)
        float x = (rand() %  200 - 100); // Range: -100 to +100
        float y = (rand() % 70 - 10);   // Range: -20 to +20
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
    Shader CubeShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_CUBE.vert","C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_CUBE.frag", nullptr);
    Shader HouseShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HOUSE_SHADER.vert","C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HOUSE_SHADER.frag", nullptr);
    Shader LightCubeShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_LIGHTING_CUBES.vert","C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_LIGHTING_CUBES.frag", nullptr);
    Shader DepthShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_SHADOW.vert","C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_SHADOW.frag", nullptr);
    Shader HeightMapShader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HEIGHTMAP.vert","C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HEIGHTMAP.frag", nullptr);
    Shader Pointshadows("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_CUBESHADOW.vert","C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_CUBESHADOW.frag", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\Geometry\\GL_CUBESHADOW.geom");
    Model Oldhouse("C:\\Users\\user\\retro\\RETRO\\rescources\\Model\\Room\\scene.gltf");//C:\Users\user\retro\RETRO\rescources\Model\they_are_here
    Model Bench1("C:\\Users\\user\\retro\\RETRO\\rescources\\Model\\Bench1\\Bench.fbx");//C:\Users\user\retro\RETRO\rescources\Model\they_are_here
    Model Tree("C:\\Users\\user\\retro\\RETRO\\rescources\\Model\\tree\\scene.gltf");//C:\Users\user\retro\RETRO\rescources\Model\they_are_here
    Model Sphere("C:\\Users\\user\\retro\\RETRO\\rescources\\Model\\sphere\\Untitled.obj");
    GLint SpecularMap = TextureFromFile("Specular.jpe", "C:\\Users\\user\\retro\\RETRO\\rescources\\textures\\Compressed");
    GLint DiffuseMap = TextureFromFile("Tiles012_4K-JPG_Color.dds", "C:\\Users\\user\\retro\\RETRO\\rescources\\textures\\Compressed");
    // GLint TerrainTexture = TextureFromFile("Grass005_4K-JPG_Displacement.dds","C:\\Users\\user\\retro\\RETRO\\rescources\\textures\\Compressed");
    HeightMap shadowscene("C:\\Users\\user\\retro\\RETRO\\rescources\\textures\\Compressed\\Tiles012_4K-JPG_Roughness.jpg" , 10.0);

    unsigned int DepthMapFBO;
    glGenFramebuffers(1,&DepthMapFBO);
    unsigned int ShadowMap;
    glBindFramebuffer(GL_FRAMEBUFFER,DepthMapFBO);
    glGenTextures(1,&ShadowMap);
    glBindTexture(GL_TEXTURE_2D,ShadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // white = not in shadow
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMap, 0);    
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE); 
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    
    glm::mat4 projectionMatrix =glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001f, 2000.0f);
    glm::mat4 LightProjection , LightView;
    glm::mat4 LightSpaceMatrix;
    LightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 30.0f);
    LightView = glm::lookAt( glm::vec3(0.0f, 10.0f, 0.0f),glm::vec3(0.0f),glm::vec3(0.0f,0.0f,-1.0f));
    LightSpaceMatrix = LightProjection * LightView;



    // Pass to shader
    DepthShader.use();
    DepthShader.setMat4("lightSpaceMatrix", LightSpaceMatrix);






    while (!glfwWindowShouldClose(window)) {
        auto startTime = hr_clock::now();
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        input.processInput(window,deltaTime,camera,FlashLight::flashlightTogglePressed,FlashLight::flashlightOn,CubeShader);

        HouseShader.use();
        glm::mat4 HouseModelMat = glm::mat4(1.0f);
        HouseModelMat = glm::scale(HouseModelMat,glm::vec3(1.0f));
        HouseShader.setMat4("model",HouseModelMat);
        HouseShader.setMat4("projection",projectionMatrix);
        HouseShader.setMat4("view",camera.GetViewMatrix());

        setDirLight(HouseShader, glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f)),glm::vec3(0.06),glm::vec3(0.8),glm::vec3(0.003));
        setSpotLight(HouseShader, "spotlight",camera.Position,camera.Front,glm::vec3(3.1f),glm::vec3(1.6f),glm::vec3(1.0f),1.0f, 0.09f, 0.032f,glm::cos(glm::radians(10.5f)),glm::cos(glm::radians(18.5f)),FlashLight::flashlightOn);
        setPointLight(HouseShader, "pointLights[0]", lightPositions[10],glm::vec3(1.7f), glm::vec3(0.5f), glm::vec3(1.0f),1.0f, 0.09f, 0.032f);     
        glm::vec3 pivot = glm::vec3(-0.346708f, 1.86082f, -0.629334f); 
        glm::mat4 SphereModel = GetRotationAroundYPoint(pivot,1.0f, 0.2f, 0.5f);
        CubeShader.use();
        CubeShader.setMat4("view",camera.GetViewMatrix());
        CubeShader.setMat4("projection",projectionMatrix);
        // SphereModel = glm::translate(SphereModel, glm::vec3(-5.0f,0.0f,0.0f));


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CubeShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, DiffuseMap);  // or Sphere-specific texture
        CubeShader.setInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, SpecularMap);
        CubeShader.setInt("texture_specular1", 1);


        HouseShader.use();
        HouseShader.setMat4("model",HouseModelMat);
        Oldhouse.RotateX(HouseShader,HouseModelMat,glm::radians(-90.0f));

        CubeShader.use();
        CubeShader.setvec3("u_ViewPos",camera.Position);

        setDirLight(CubeShader, glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f)),glm::vec3(0.3),glm::vec3(0.008),glm::vec3(0.003));
        setSpotLight(CubeShader, "spotlight",camera.Position,camera.Front,glm::vec3(2.1f),glm::vec3(0.8f),glm::vec3(0.01f),1.0f, 0.09f, 0.032f,glm::cos(glm::radians(10.5f)),glm::cos(glm::radians(18.5f)),FlashLight::flashlightOn);
        setPointLight(CubeShader, "pointLights[0]", lightPositions[10],glm::vec3(1.7f), glm::vec3(0.5f), glm::vec3(0.4f),1.0f, 0.09f, 0.032f);     

        LightCubeShader.use();
        glm::mat4 LightCubeMatrix = glm::mat4(1.0f);
        LightCubeMatrix = glm::translate(LightCubeMatrix,lightPositions[10]);
        setMatrices(LightCubeShader,LightCubeMatrix, projectionMatrix ,camera.GetViewMatrix());
        glBindVertexArray(CUBEVAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);

        CubeShader.use();
        glBindVertexArray(CUBEVAO);
        glDrawArraysInstanced(GL_TRIANGLES,0,36,amount);
        glBindVertexArray(0);



        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // enable wireframe
        shadowscene.Draw(HeightMapShader,glm::mat4(1.0f),camera.GetViewMatrix(),projectionMatrix);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // disable wireframe

        

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

