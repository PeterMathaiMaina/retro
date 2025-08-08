#pragma once

#include <GL/glew.h>
#include <map>
#include <iostream>
#include <string>
#include "../graphics/Model.hpp"
#include "../../textureLoader/textureLoader.hpp"



struct AssetManager{
    static void Init();
    static Model& GetModelByName(const std::string& name);
    static unsigned int GetTextureByName(const std::string& name);
private:
    static std::map<std::string,Model>Models;
    static std::map<std::string,unsigned int >Textures;
};