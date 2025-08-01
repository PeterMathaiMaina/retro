#include "GlobalDef.h"
#include "ShaderManager.hpp"

// Define global shader pointers here
Shader* HouseShader = nullptr;
Shader* BunnyShader = nullptr;
Shader* CubeShader = nullptr;
Shader* HeightMapShader = nullptr;
Shader* HDRShader = nullptr;
Shader* LightingCubeShader = nullptr;
Shader* TreeShader = nullptr;

void GlobalDef::GlobalInit() {
    ShaderManager::InitShaders();

    HouseShader = &ShaderManager::Getshaderbyname("m_House");
    BunnyShader = &ShaderManager::Getshaderbyname("m_Bunny");
    CubeShader = &ShaderManager::Getshaderbyname("m_Cube");
    HeightMapShader = &ShaderManager::Getshaderbyname("m_HeightMap");
    HDRShader = &ShaderManager::Getshaderbyname("m_HDR");
    LightingCubeShader = &ShaderManager::Getshaderbyname("m_LightingCube");
    TreeShader = &ShaderManager::Getshaderbyname("m_Tree");
}
