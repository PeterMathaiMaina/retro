#ifndef CLEANUP_H
#define CLEANUP_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
namespace GL_cleanUp {
    void cleanup_array_buffer(unsigned int& VAO, unsigned int& VBO){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        //std::cout<<"DELETED THE SHIT"<<'\n';
    }
}

#endif
