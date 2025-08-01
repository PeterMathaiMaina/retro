#pragma once

#include "Shader.hpp"

extern Shader* HouseShader;  
extern Shader* BunnyShader;  
extern Shader* CubeShader;  
extern Shader* HeightMapShader;  
extern Shader* HDRShader;  
extern Shader* LightingCubeShader;  
extern Shader* TreeShader;  


class GlobalDef {
public:
    static void GlobalInit(); 
};
