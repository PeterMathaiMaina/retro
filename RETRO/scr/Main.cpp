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




float lastFrame = 0.0f;
using hr_clock = std::chrono::high_resolution_clock;
auto lastFrameTime = hr_clock::now();
const double targetFPS = 60.0;
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


    GLuint  grassTexture= TextureFromFile("grass.dds", "/home/mathai/retro/RETRO/rescources/textures/Compressed");
    GLuint transparentWindow= TextureFromFile("blending_transparent_window.dds", "/home/mathai/retro/RETRO/rescources/textures/Compressed");
    GLuint WoodTexture = TextureFromFile("container.dds","/home/mathai/retro/RETRO/rescources/textures/Compressed");
    GLuint Wall = TextureFromFile("SecondWall.dds","/home/mathai/retro/RETRO/rescources/textures/Compressed");
    GLuint cubeMap  = loadKTXCubemap("/home/mathai/retro/RETRO/rescources/textures/Compressed/skybox/skybox.ktx");   
    float cubeVertices[] = 
    {
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
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
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
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
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

    std::vector<glm::vec3> windows = {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)
    };
    std::vector<std::string> faces = {
        "/home/mathai/retro/RETRO/rescources/textures/Compressed/skybox/back.dds",
        "/home/mathai/retro/RETRO/rescources/textures/Compressed/skybox/front.dds",
        "/home/mathai/retro/RETRO/rescources/textures/Compressed/skybox/bottom.dds",
        "/home/mathai/retro/RETRO/rescources/textures/Compressed/skybox/left.dds",
        "/home/mathai/retro/RETRO/rescources/textures/Compressed/skybox/right.dds",
        "/home/mathai/retro/RETRO/rescources/textures/Compressed/skybox/top.dds"
    };

    //unsigned int cubemapTexture = loadCompressedCubemap(faces);

    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);


    //unsigned int SkyBoxVOA, SkyBoxVBO;
    //glGenVertexArrays(1,&SkyBoxVOA);
    //glGenBuffers(1,&SkyBoxVBO);
    //glBindVertexArray(SkyBoxVOA);
    //glBindBuffer(GL_ARRAY_BUFFER,SkyBoxVBO);
    //glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);
    //glEnableVertexAttribArray(0);    
    //glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),nullptr);
    //glBindBuffer(GL_ARRAY_BUFFER,0);
    //glBindVertexArray(0);

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    unsigned int grassVAO, grassVBO;
    glGenVertexArrays(1, &grassVAO);
    glGenBuffers(1, &grassVBO);

    glBindVertexArray(grassVAO);
    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    // TexCoord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    unsigned int FBO;
    glGenFramebuffers(1,&FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);


    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER,0);


    unsigned int FrameQuadVOA, FrameQuadVBO;
    glGenVertexArrays(1, &FrameQuadVOA);
    glGenBuffers(1, &FrameQuadVBO);
    glBindVertexArray(FrameQuadVOA);
    glBindBuffer(GL_ARRAY_BUFFER, FrameQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);    
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4* sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);






    Shader AlphaShader("/home/mathai/retro/RETRO/rescources/Shaders/GL_ALPHA_SHADER.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_ALPHA_SHADER.frag");
    Shader FrameBuffershader("/home/mathai/retro/RETRO/rescources/Shaders/GL_FRAMEBUFFER.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_FRAMEBUFFER.frag");
    Shader SkyBox_shader("/home/mathai/retro/RETRO/rescources/Shaders/GL_SKYBOX_SHADER.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_SKYBOX_SHADER.frag");
    //AlphaShader.use();

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Input input;

    //glm::mat4 projectionMatrix=glm::perspective(glm::radians(camera.Zoom), (float)955 / (float)560, 0.001f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        auto startTime = hr_clock::now();
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        input.processInput(window,camera.Position, camera.Front, camera.Up, deltaTime, camera);
             

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),800.0f / 600.0f,0.1f, 100.0f);
        // --------------------------- First pass: Render scene to FBO ---------------------------
        //glBindFramebuffer(GL_FRAMEBUFFER, FBO); 
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glEnable(GL_DEPTH_TEST);

        // --- Render opaque cube
        
        //AlphaShader.use();
        //AlphaShader.setMat4("u_Model", model);
        //AlphaShader.setMat4("u_View", camera.GetViewMatrix());
        //AlphaShader.setMat4("u_Projection", projection);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, Wall);
        //AlphaShader.setInt("u_DiffuseTexture", 0);
        //glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        // --------------------------- Second pass: Render to screen ---------------------------
        //glBindFramebuffer(GL_FRAMEBUFFER, 0); 
        //glDisable(GL_DEPTH_TEST);

        //FrameBuffershader.use();
        //float offset = 1.0f / 1000; // or average of width and height
        //FrameBuffershader.use();
        //FrameBuffershader.setFloat("offset", offset);

        //glBindVertexArray(FrameQuadVOA); 
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture);
        //FrameBuffershader.setInt("screenTexture", 0);
        //glDrawArrays(GL_TRIANGLES, 0, 6); 
        //glBindVertexArray(0);



        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        SkyBox_shader.use();
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        SkyBox_shader.setMat4("view", view);
        SkyBox_shader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, WoodTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default



        //---------------------------------------FRAME CAPPING---------------------------------------------------------
        auto endTime = hr_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;
        double frameTime = elapsed.count(); 
        if (frameTime < targetFrameDuration) {
            std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameDuration - frameTime));
        }
        lastFrame = currentFrame;
        //--------------------------------------------------------------------------FrameQuadVOA------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    GL_cleanUp::cleanup_array_buffer(cubeVAO,cubeVBO);
    GL_cleanUp::cleanup_array_buffer(grassVAO,grassVBO);
    GL_cleanUp::cleanup_array_buffer(FrameQuadVOA,FrameQuadVBO);
    GL_cleanUp::cleanup_array_buffer(skyboxVAO,skyboxVBO);
    
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
