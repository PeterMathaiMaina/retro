#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct Shader
{
    unsigned int ID; // Shader program ID

    // Constructor generates the shader on the fly
    Shader(const char* vertexPath, const char* fragmentPath);
    
    // Activate the shader program
    void use();
    
    // Setters for different uniform types
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    // Utility function to check shader compilation/linking errors
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
