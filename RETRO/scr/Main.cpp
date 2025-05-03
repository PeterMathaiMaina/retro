#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

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


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
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

    GLFWwindow* window = glfwCreateWindow(1000, 560, "RETRO", nullptr, nullptr);
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

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    Shader modelShader("/home/mathai/retro/RETRO/rescources/Shaders/model.vert","/home/mathai/retro/RETRO/rescources/Shaders/model.frag");


    // Setup input callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input input;



    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1.78f, 0.1f, 100.0f);
    Model house("/home/mathai/retro/RETRO/rescources/Models/objects/house/Abandoned_House.obj");
    //Model glock("/home/mathai/retro/RETRO/rescources/Models/objects/Glock/model.dae");
    //Model shotgun("/home/mathai/retro/RETRO/rescources/Models/objects/shotgun/sketchfab_m12.fbx");
    //Model room("/home/mathai/retro/RETRO/rescources/Models/scene/Room/room.obj");
    //TextureFromFile("Scene_-_Root_baseColor.jpg","/home/mathai/retro/RETRO/rescources/Models/objects/Backpack");

    //TextureFromFile("house_color.png","/home/mathai/retro/RETRO/rescources/Models/objects/house");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        input.processInput(window, modelShader, camera.Position, camera.Front, camera.Up, deltaTime, camera);   
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // 2. Set the transformation matrices
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        // Apply any translations, rotations, and scaling to the modelMatrix here
        modelShader.setMat4("u_Model", modelMatrix);

        glm::mat4 viewMatrix = camera.GetViewMatrix();
        modelShader.setMat4("u_View", viewMatrix);

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)16 / (float)9, 0.1f, 100.0f);
        modelShader.setMat4("u_Projection", projectionMatrix);

        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
        modelShader.setMat4("u_scaleMatrix",scaleMatrix);


        // 3. Set the texture samplers
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        modelShader.setInt("u_DiffuseTexture", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 1);
        modelShader.setInt("u_SpecularTexture", 1);

        // 4. Set lighting uniforms
        glm::vec3 lightPosition(camera.Position);
        modelShader.setvec3("u_LightPos", lightPosition);

        glm::vec3 lightColor(1.0f,1.0f, 1.0f);
        modelShader.setvec3("u_LightColor", lightColor);

        // 5. Set view position and specular strength
        glm::vec3 cameraPosition = camera.Position;
        modelShader.setvec3("u_ViewPos", cameraPosition);

        float specularStrength = 0.001f;
        modelShader.setFloat("u_SpecularStrength", specularStrength);
        modelShader.setFloat("light.constant",  1.0f);
        modelShader.setFloat("light.linear",    0.09f);
        modelShader.setFloat("light.quadratic", 0.032f);	    
        
        //gun.Draw(modelShader);
        house.Draw(modelShader);
        //shotgun.Draw(modelShader);
        //church.Draw(modelShader);

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
