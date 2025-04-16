#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Graphics/Shader.hpp"
#include "Utils/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Input/Input.hpp"
#include "Camera/Camera.hpp"

// Global variables
float lastX = 960.0f / 2.0f;
float lastY = 560.0f / 2.0f;
bool firstMouse = true;
float fov = 55.0f;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), 
              glm::vec3(0.0f, 1.0f, 0.0f), 
              -90.0f, 
              0.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) ;



void loadTexture(const char* path, unsigned int& texture) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, (nrChannels == 4) ? GL_RGBA : GL_RGB, width, height, 0, (nrChannels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture at " << path << std::endl;
    }
    stbi_image_free(data);
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(960, 560, "RETRO", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing GLEW!" << std::endl;
        return -1;
    }


    // Define cube vertices
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f, //0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f, //1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.5f,  0.5f,  0.5f, //1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.5f,  0.5f,  0.5f, //1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.5f,  0.5f,  0.5f, //0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f, //0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,0.5f, -0.5f,  0.5f,  //0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,0.5f, -0.5f,  0.5f,  //1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,0.5f,  0.5f,  0.5f,  //1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,0.5f,  0.5f,  0.5f,  //1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,0.5f,  0.5f,  0.5f,  //0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,0.5f, -0.5f,  0.5f,  //0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 0.5f, -0.5f,  0.5f, //1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.5f, -0.5f,  0.5f, //1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.5f,  0.5f,  0.5f, //0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.5f,  0.5f,  0.5f, //0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f, //0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.5f, -0.5f,  0.5f, //1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 0.5f, -0.5f,  0.5f, //1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.5f, -0.5f,  0.5f, //1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.5f,  0.5f,  0.5f, //0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.5f,  0.5f,  0.5f, //0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f, //0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.5f, -0.5f,  0.5f, //1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,0.5f, -0.5f,  0.5f,  //0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,0.5f, -0.5f,  0.5f,  //1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,0.5f,  0.5f,  0.5f,  //0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,0.5f, -0.5f,  0.5f,  //0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.5f, -0.5f,  0.5f, //0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.5f, -0.5f,  0.5f, //1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f, //1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f, //1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f, //0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.5f, -0.5f,  0.5f, //0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        //glm::vec3( 2.0f,  5.0f, -15.0f),
        //glm::vec3(-1.5f, -2.2f, -2.5f),
        //glm::vec3(-3.8f, -2.0f, -12.3f),
        //glm::vec3( 2.4f, -0.4f, -3.5f),
        //glm::vec3(-1.7f,  3.0f, -7.5f),
        //glm::vec3( 1.3f, -2.0f, -2.5f),
        //glm::vec3( 1.5f,  2.0f, -2.5f),
        //glm::vec3( 1.5f,  0.2f, -1.5f),
        //glm::vec3(-1.3f,  1.0f, -1.5f),
        //glm::vec3( 4.0f,  1.0f, -10.0f),
        //glm::vec3(-3.0f, -1.5f, -6.0f),
        glm::vec3( 2.5f,  3.5f, -8.5f),
        //glm::vec3(-5.5f,  0.0f, -4.0f),
        //glm::vec3( 1.0f, -3.0f, -7.0f),
        //glm::vec3(-2.0f,  2.0f, -3.5f),
        //glm::vec3( 3.0f, -2.5f, -9.0f),
        //glm::vec3( 0.5f,  4.0f, -5.0f),
        //glm::vec3(-4.5f,  1.5f, -6.5f),
        glm::vec3( 2.0f, -1.0f, -2.0f)
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int texture1, texture2;
    loadTexture("/home/ender/retro/RETRO/scr/Images/container.jpg", texture1);
    loadTexture("/home/ender/retro/RETRO/scr/Images/awesomeface.png", texture2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    //Shader shaderProgram("/home/ender/retro/RETRO/scr/Shaders/lightning.vert", "/home/ender/retro/RETRO/scr/Shaders/lighting.frag");
    //shaderProgram.use();
    Shader lightningShader("/home/ender/retro/RETRO/scr/Shaders/lighting.vert", "/home/ender/retro/RETRO/scr/Shaders/lighting.frag");
    //shaderProgram.setInt("texture1", 0);
    //shaderProgram.setInt("texture2", 1);
    unsigned int lightVAO,lightingVBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glGenBuffers(1,&lightingVBO);
    float lightingVertices[]
    {
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
    };

    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, lightingVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(lightingVertices),lightingVertices,GL_STATIC_DRAW);
    // set the vertex attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    Input input;
    glEnable(GL_DEPTH_TEST);

    
    // Set input callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(fov), 960.0f / 560.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        lightningShader.use();
        lightningShader.setMat4("view", view);
        lightningShader.setMat4("projection", projection);
        lightningShader.setFloat("mixture", 0.2f);
        glm::vec3 cameraFront {0.0f,0.0f,-1.0f};
        input.processInput(window, lightningShader, camera.Position, cameraFront, camera.Up, deltaTime, camera);

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture1);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(lightVAO);

        for (unsigned int i = 0; i < 3; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightningShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

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