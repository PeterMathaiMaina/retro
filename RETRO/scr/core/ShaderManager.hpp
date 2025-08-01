#pragma once


#include "Shader.hpp"
#include <map>
#include <string>


class ShaderManager
{
public:
    static void InitShaders();
    static Shader& Getshaderbyname(const std::string& name); 
private:
    static std::map<std::string,Shader>Shaders;
};


