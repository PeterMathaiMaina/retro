#include "ShaderManager.hpp"
#include <iostream>
std::map<std::string, Shader> ShaderManager::Shaders;
///home/peter/retro/RETRO/rescources/Shaders/GL_CUBE.vert
void ShaderManager::InitShaders() {
    Shaders.emplace("m_Cube" , Shader("home/peter/retro/RETRO/rescources/Shaders/GL_CUBE.vert", "home/peter/retro\\RETRO/rescources/Shaders/GL_CUBE.frag", nullptr));
    Shaders.emplace("m_Tree" , Shader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_TREES.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_TREES.frag", nullptr));
    Shaders.emplace("m_Bunny", Shader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_BUNNY.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_BUNNY.frag", nullptr));
    Shaders.emplace("m_HeightMap",Shader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HEIGHTMAP.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HEIGHTMAP.frag", nullptr));
    Shaders.emplace("m_LightingCube",Shader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_LIGHTING_CUBES.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_LIGHTING_CUBES.frag", nullptr));
    Shaders.emplace("m_House",Shader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HOUSE_SHADER.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HOUSE_SHADER.frag", nullptr));
    Shaders.emplace("m_HDR",Shader("C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HDR.vert", "C:\\Users\\user\\retro\\RETRO\\rescources\\Shaders\\GL_HDR.frag", nullptr));
}

Shader& ShaderManager::Getshaderbyname(const std::string& name)
{
    auto it =Shaders.find(name);
    if (it ==Shaders.end()){
        std::cerr<<"shader not found"<< " " <<name<<std::endl;
        std::exit(EXIT_FAILURE);
    }
    return it->second;
}
