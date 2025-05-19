#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
//#include <string>
#include "core/Shader.hpp"
#include "../third_party/imageprocessing/stb_image.h"
#include "../third_party/glm/glm.hpp"
#include "../third_party/glm/gtc/matrix_transform.hpp"
#include "../third_party/glm/gtc/type_ptr.hpp"
#include "Input/Input.h"
#include "Camera/Camera.hpp"
#include "graphics/Mesh.hpp"
#include "../textureLoader/textureLoader.hpp"
#include "graphics/Model.hpp"

float lastFrame = 0.0f;
bool SpotLightOn = true;


Camera camera(glm::vec3(0.0f,0.3f, 0.3f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float lastX = 960.0f / 2.0f;
float lastY = 560.0f / 2.0f;
bool firstMouse = true;
float fov = 55.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main() {
    // GLFW initialization
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(950, 600, "RETRO", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    std::cout<<"IN THIS WORLD WE ARE HELD CAPTIVE BY THE LIMITATIONS OF OUR OWN THOUGHTS. "<<'\n'<<'\n'<<'\n'<<std::endl;



    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_STENCIL_TEST);
    //glDepthFunc(GL_ALWAYS);

    Shader modelShader("/home/mathai/retro/RETRO/rescources/Shaders/model.vert","/home/mathai/retro/RETRO/rescources/Shaders/model.frag");
    Shader OutlineShader("/home/mathai/retro/RETRO/rescources/Shaders/shaderSingleColor.vert","/home/mathai/retro/RETRO/rescources/Shaders/shaderSingleColor.frag");



    // Setup input callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input input;

    Model Tree("/home/mathai/retro/RETRO/rescources/Models/objects/Tree/Tree.obj");
    Model Gun("/home/mathai/retro/RETRO/rescources/Models/objects/Gun/m4a1_s.obj");
    Model Lamp("/home/mathai/retro/RETRO/rescources/Models/scene/flat-surface/Flat Surface.stl");


    glm::mat4 projectionMatrix=glm::perspective(glm::radians(camera.Zoom), (float)955 / (float)560, 0.001f, 100.0f);

    modelShader.setMat4("u_Projection", projectionMatrix);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        
        
        input.processInput(window, modelShader, camera.Position, camera.Front, camera.Up, deltaTime, camera);   
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //setting the lighting uniforms
        //directional light
        modelShader.setvec3("dirlight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        modelShader.setvec3("dirlight.ambient",glm::vec3(0.3f, 0.24f, 0.14f));
        modelShader.setvec3("dirlight.diffuse",glm::vec3(0.7f, 0.42f, 0.26f));
        modelShader.setvec3("dirlight.specular",glm::vec3(0.5f, 0.5f, 0.5f));
        //spot-light
        modelShader.setvec3("spotlight.position", camera.Position);
        modelShader.setvec3("spotlight.direction", camera.Front);
        modelShader.setFloat("spotlight.cutOff", glm::cos(glm::radians(16.5f)));
        modelShader.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(30.5f)));         
        modelShader.setvec3("spotlight.ambient",  glm::vec3(0.2f));
        modelShader.setvec3("spotlight.diffuse",  glm::vec3(0.5f));
        modelShader.setvec3("spotlight.specular", glm::vec3(0.001f));
        modelShader.setFloat("spotlight.constant", 1.0f);
        modelShader.setFloat("spotlight.linear", 0.09f);
        modelShader.setFloat("spotlight.quadratic", 0.032f);
        modelShader.setBool("spotlight.enabled", SpotLightOn);
        //point-light
        for (int i = 0; i < 1; ++i) {
            std::string number = std::to_string(i);
            modelShader.setvec3("pointLights[" + number + "].position",glm::vec3( -3.00159f,  -4.36768f,  -4.08302f));
            modelShader.setvec3("pointLights[" + number + "].ambient", glm::vec3(1.0f, 1.0f, 1.0f) * 0.005f);
            modelShader.setvec3("pointLights[" + number + "].diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
            modelShader.setvec3("pointLights[" + number + "].specular", glm::vec3(0.00000002f));
            modelShader.setFloat("pointLights[" + number + "].constant", 1.0f);
            modelShader.setFloat("pointLights[" + number + "].linear", 0.09f);
            modelShader.setFloat("pointLights[" + number + "].quadratic", 0.032f);
        }

        //Set the transformation matrices
        glm::mat4 viewMatrix = camera.GetViewMatrix();
        modelShader.setMat4("u_View", viewMatrix);

        glm::vec3 cameraPosition = camera.Position;
        modelShader.setvec3("u_ViewPos", cameraPosition);        


        // 3. Set the texture samplers
        //diffuse textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        modelShader.setInt("u_DiffuseTexture", 0);
        //specular textures
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 1);
        modelShader.setInt("u_SpecularTexture", 1);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));        
        model = glm::scale(model, glm::vec3(0.1f));
        modelShader.setMat4("u_Model", model);



        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // Always pass, write 1 to stencil buffer
        glStencilMask(0xFF); // Enable writing to stencil
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            Tree.Draw(modelShader);      
            
            
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Draw only where stencil != 1
        glStencilMask(0x00); // Disable stencil writing
        glDisable(GL_DEPTH_TEST); // Optional: avoid z-fighting
        glm::mat4 ModelMatrix = glm::mat4(1.0); 
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f,0.0f,0.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.12f));
        OutlineShader.setMat4("u_View", viewMatrix);
        OutlineShader.setMat4("u_Projection", projectionMatrix);
        OutlineShader.setMat4("Model_SingleCol",ModelMatrix);
        Tree.Draw(OutlineShader);
        //Gun.Draw(modelShader);
        glStencilMask(0xFF); // Re-enable stencil writing
        glEnable(GL_DEPTH_TEST);

        std::cout<<"X: "<<camera.Position.x<<"Y: "<<camera.Position.y<<"Z: "<<camera.Position.z<<'\n';

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
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 90.0f)
        fov = 90.0f;
}