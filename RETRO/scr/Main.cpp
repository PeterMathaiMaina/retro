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
#include <bitset>




float lastFrame = 0.0f;
using hr_clock = std::chrono::high_resolution_clock;
auto lastFrameTime = hr_clock::now();
const double targetFPS = 90.0;
const double targetFrameDuration = 1.0 / targetFPS; // ~0.01667 seconds (16.67 ms)
Camera camera(glm::vec3(0.0f,0.3f, 1.2f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float lastX = 960.0f / 2.0f;
float lastY = 560.0f / 2.0f;
bool firstMouse = true;
float fov = 55.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

namespace FlashLight
{
    bool flashlightTogglePressed = false;
    bool flashlightOn = false; 
}
struct CameraData {
    glm::mat4 view;
    glm::mat4 projection;
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }


    Model Chair("/home/mathai/retro/RETRO/rescources/Models/objects/Chair/scene.gltf");
    Model Rock("/home/mathai/retro/RETRO/rescources/Models/objects/rock/rock.obj");
    Model Planet("/home/mathai/retro/RETRO/rescources/Models/objects/planet/planet.obj");


    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    };
    float ExtracubeVertices[]
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float QuadVertices[] = {
        // positions        // texture coords
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  // top-left
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // bottom-left
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // bottom-right

        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  // top-left
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // bottom-right
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f   // top-right
    };
    float Data[]
    {
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
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

    
    Shader CubeShader("/home/mathai/retro/RETRO/rescources/Shaders/GL_CUBE.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_CUBE.frag",nullptr);
    Shader ModelShader("/home/mathai/retro/RETRO/rescources/Shaders/GL_MODEL_COMP.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_MODEL_COMP.frag",nullptr);
    Shader NormalVisuals("/home/mathai/retro/RETRO/rescources/Shaders/GL_NORMALS.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_NORMALS.frag","/home/mathai/retro/RETRO/rescources/Shaders/Geometry/GL_MODEL_GEO.geom"); 
    Shader Rock_Shader("/home/mathai/retro/RETRO/rescources/Shaders/GL_ROCKS.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_ROCKS.frag",nullptr);
        

    glm::vec3 translations[20];
    unsigned int index = 0;
    float offset = 1.0f;
    unsigned int amount = 1000;
    glm::mat4 *modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed	
    float radius = 25.0;
    float offsetRot = 2.5f;
    for(unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
    
        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));
    
        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
    
        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }  

    unsigned int InstanceVBO; 
    glGenBuffers(1, &InstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for(unsigned int i = 0; i < Rock.meshes.size(); i++)
    {
        unsigned int VAO = Rock.meshes[i].VAO; // Assuming rock.meshes[i].VAO holds the VAO ID for each mesh
        glBindVertexArray(VAO);
        // vertex attributes
        std::size_t vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
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
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    framebuffer_size_callback(window,1000,650);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Input input;

    while (!glfwWindowShouldClose(window)) {
        auto startTime = hr_clock::now();
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;

        input.processInput(window,deltaTime,camera,FlashLight::flashlightTogglePressed,FlashLight::flashlightOn);


        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projectionMatrix =glm::perspective(glm::radians(camera.Zoom), (float)955 / (float)560, 0.001f, 1000.0f);

        Rock_Shader.setMat4("u_Projection",projectionMatrix);
        Rock_Shader.setMat4("u_View",camera.GetViewMatrix());



        ModelShader.setvec3("dirlight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        ModelShader.setvec3("dirlight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
        ModelShader.setvec3("dirlight.diffuse", glm::vec3(0.001f, 0.01f, 0.001f));
        ModelShader.setvec3("dirlight.specular", glm::vec3(0.01f, 0.01f, 0.001f));
        ModelShader.setvec3("spotlight.ambient", glm::vec3(0.0000000f));
        ModelShader.setvec3("spotlight.diffuse", glm::vec3(0.5f));
        ModelShader.setvec3("spotlight.specular", glm::vec3(1.5f));
        ModelShader.setFloat("spotlight.constant", 1.0f);
        ModelShader.setFloat("spotlight.linear", 0.09f);
        ModelShader.setFloat("spotlight.quadratic", 0.32f);
        ModelShader.setvec3("spotlight.position", camera.Position);
        ModelShader.setvec3("spotlight.direction", camera.Front);
        ModelShader.setFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
        ModelShader.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(30.5f)));
        ModelShader.setBool("spotlight.enabled", FlashLight::flashlightOn);
        for (int i = 0; i < 4; ++i) {
        std::string number = std::to_string(i);
        ModelShader.setvec3("pointLights[" + number + "].position", lightPositions[i]);
        ModelShader.setvec3("pointLights[" + number + "].ambient", glm::vec3(1.0f) * 0.2f);
        ModelShader.setvec3("pointLights[" + number + "].diffuse", glm::vec3(0.0002f));
        ModelShader.setvec3("pointLights[" + number + "].specular", glm::vec3(0.005f));
        ModelShader.setFloat("pointLights[" + number + "].constant", 1.0f);
        ModelShader.setFloat("pointLights[" + number + "].linear", 0.09f);
        ModelShader.setFloat("pointLights[" + number + "].quadratic", 0.032f);
        }
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        ModelShader.setMat4("u_View", camera.GetViewMatrix());
        ModelShader.setvec3("u_ViewPos",camera.Position);
        ModelShader.setMat4("u_Projection", projectionMatrix);  
        ModelShader.setMat4("u_Model",ModelMatrix);

        for(unsigned int i = 0; i < Rock.meshes.size(); i++)
        {
            Rock_Shader.use();
            glBindVertexArray(Rock.meshes[i].VAO);
            glDrawElementsInstanced(
                GL_TRIANGLES, Rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
            );
        }

        //---------------------------------------CAPPING---------------------------------------------------------
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
