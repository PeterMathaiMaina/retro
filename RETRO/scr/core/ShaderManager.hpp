#pragma once


#include "Shader.hpp"
#include <map>
#include <string>


class ShaderManager
{
public:
    static std::map<std::string,Shader>Shaders;
    static void Init();
    static Shader& Getshaderbyname(const std::string& name); 
private:
};


