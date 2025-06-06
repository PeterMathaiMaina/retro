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
    //GLuint cubeMap  = loadKTXCubemap("/home/mathai/retro/RETRO/rescources/textures/Compressed/skybox/skybox.ktx");   
    Model Chair("/home/mathai/retro/RETRO/rescources/Models/objects/Chair/scene.gltf");
    Model Table("/home/mathai/retro/RETRO/rescources/Models/objects/Table/scene.gltf");


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
    glm::vec3 pointLightPositions[] = {
    glm::vec3(-3.0f,  0.0f, -3.0f),
    glm::vec3( 3.0f,  0.0f, -3.0f),
    glm::vec3(-3.0f,  0.0f,  3.0f),
    glm::vec3( 3.0f,  0.0f,  3.0f)
    };


    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    //method1

    glBufferData(GL_ARRAY_BUFFER,sizeof(Data),nullptr,GL_STATIC_DRAW);
    void *ptr = glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
    memcpy(ptr,Data,sizeof(Data));
    glUnmapBuffer(GL_ARRAY_BUFFER); 

    //method 2
    
    //glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices)+sizeof(ExtracubeVertices), &cubeVertices, GL_STATIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER,sizeof(cubeVertices),sizeof(ExtracubeVertices),ExtracubeVertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);


    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    //glBindBuffer(GL_COPY_READ_BUFFER,cubeVBO);
    //glBindBuffer(GL_COPY_WRITE_BUFFER,skyboxVBO);
    //glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER, 0, 0, 5 * sizeof(float));
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    //Shader AlphaShader("/home/mathai/retro/RETRO/rescources/Shaders/GL_ALPHA_SHADER.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_ALPHA_SHADER.frag");
    Shader FrameBuffershader("/home/mathai/retro/RETRO/rescources/Shaders/GL_FRAMEBUFFER.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_FRAMEBUFFER.frag");
    Shader SkyBox_shader("/home/mathai/retro/RETRO/rescources/Shaders/GL_SKYBOX_SHADER.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_SKYBOX_SHADER.frag");
    Shader CubeShader("/home/mathai/retro/RETRO/rescources/Shaders/GL_CUBE.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_CUBE.frag");
    Shader ModelShader("/home/mathai/retro/RETRO/rescources/Shaders/GL_MODEL_COMP.vert","/home/mathai/retro/RETRO/rescources/Shaders/GL_MODEL_COMP.frag");
    //AlphaShader.use();

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Input input;




    while (!glfwWindowShouldClose(window)) {
        auto startTime = hr_clock::now();
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        //Input Processing 

        
        input.processInput(window,deltaTime,camera);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(1.0f,1.0f,1.0f));  
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1000.0f / 650.0f, 0.1f, 500.0f);


        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         ModelShader.setvec3("dirlight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        ModelShader.setvec3("dirlight.ambient", glm::vec3(0.3f, 0.24f, 0.14f));
        ModelShader.setvec3("dirlight.diffuse", glm::vec3(0.7f, 0.42f, 0.26f));
        ModelShader.setvec3("dirlight.specular", glm::vec3(0.5f, 0.5f, 0.5f));



        ModelShader.setvec3("spotlight.ambient", glm::vec3(0.2f));
        ModelShader.setvec3("spotlight.diffuse", glm::vec3(0.5f));
        ModelShader.setvec3("spotlight.specular", glm::vec3(0.2f));
        ModelShader.setFloat("spotlight.constant", 1.0f);
        ModelShader.setFloat("spotlight.linear", 0.09f);
        ModelShader.setFloat("spotlight.quadratic", 0.032f);
        //ModelShader.setBool("spotlight.enabled", SpotLightOn);


        ModelShader.setvec3("spotlight.position", camera.Position);
        ModelShader.setvec3("spotlight.direction", camera.Front);
        ModelShader.setFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
        ModelShader.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(30.5f)));

        for (int i = 0; i < 4; ++i) {
        std::string number = std::to_string(i);
        ModelShader.setvec3("pointLights[" + number + "].position", pointLightPositions[i]);
        ModelShader.setvec3("pointLights[" + number + "].ambient", glm::vec3(1.0f) * 0.05f);
        ModelShader.setvec3("pointLights[" + number + "].diffuse", glm::vec3(0.8f));
        ModelShader.setvec3("pointLights[" + number + "].specular", glm::vec3(1.0f));
        ModelShader.setFloat("pointLights[" + number + "].constant", 1.0f);
        ModelShader.setFloat("pointLights[" + number + "].linear", 0.09f);
        ModelShader.setFloat("pointLights[" + number + "].quadratic", 0.032f);
        }



        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        //ModelMatrix = glm::scale(ModelMatrix,glm::vec3(0.1f));
        

        ModelShader.setMat4("u_View", camera.GetViewMatrix());


        ModelShader.setvec3("u_ViewPos",camera.Position);

        glm::mat4 projectionMatrix=glm::perspective(glm::radians(camera.Zoom), (float)955 / (float)560, 0.001f, 100.0f);

        ModelShader.setMat4("u_Projection", projectionMatrix);  

        ModelShader.setMat4("u_Model",ModelMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        ModelShader.setInt("u_DiffuseTexture", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 1);
        ModelShader.setInt("u_SpecularTexture", 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 2);
        ModelShader.setInt("u_MetallicTexture", 2);
        Chair.Translate(ModelShader,ModelMatrix,glm::vec3(0.0,0.0,0.0));
        //Chair.RotateX(ModelShader,ModelMatrix,)
        float angleY =glm::radians(270.0f); 
        Chair.RotateY(ModelShader,ModelMatrix,angleY);                
        float c_ScaleFactor = 0.2f;
        Chair.Scale(ModelShader,ModelMatrix,c_ScaleFactor);
        Chair.Draw(ModelShader);
        ModelMatrix = glm::mat4(1.0f);
        Table.Translate(ModelShader,ModelMatrix,glm::vec3(0.0,0.0,1.0));
        float t_ScaleFactor = 0.003f;
        Table.Scale(ModelShader,ModelMatrix,t_ScaleFactor);
        float t_angleY =glm::radians(90.0f); 

        Table.RotateY(ModelShader,ModelMatrix,t_angleY);
        Table.Draw(ModelShader);



        //---------------------------------------FRAME CAPPING---------------------------------------------------------
        auto endTime = hr_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;
        double frameTime = elapsed.count(); 
        if (frameTime < targetFrameDuration) {
            std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameDuration - frameTime));
        }
        lastFrame = currentFrame;
        //std::cout<<" X: "<<camera.Position.x<<" Y: "<<camera.Position.y<<" Z: "<<camera.Position.z<<'\n';
        //std::cout << "data in bits: " << std::bitset<32>(sizeof(cubeVertices)) << std::endl;
        //std::cout << sizeof(cubeVertices) <<std::endl;
        //--------------------------------------------------------------------------FrameQuadVOA------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    GL_cleanUp::cleanup_array_buffer(cubeVAO,cubeVBO);
    GL_cleanUp::cleanup_array_buffer(skyboxVAO,skyboxVBO);
    //GL_cleanUp::cleanup_array_buffer(cubeVAO,cubeVBO);
    
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
