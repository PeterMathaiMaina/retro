#include "GlobalAssets.h"

// Define and initialize here
Shader* GlobalAssets::CubeShader         = nullptr;
Shader* GlobalAssets::TreeShader         = nullptr;
Shader* GlobalAssets::BunnyShader        = nullptr;
Shader* GlobalAssets::HeightMapShader    = nullptr;
Shader* GlobalAssets::LightingCubeShader = nullptr;
Shader* GlobalAssets::HouseShader        = nullptr;
Shader* GlobalAssets::HDRShader          = nullptr;

void GlobalAssets::Init() {
    CubeShader         = &ShaderManager::Getshaderbyname("m_Cube");
    TreeShader         = &ShaderManager::Getshaderbyname("m_Tree");
    BunnyShader        = &ShaderManager::Getshaderbyname("m_Bunny");
    HeightMapShader    = &ShaderManager::Getshaderbyname("m_HeightMap");
    LightingCubeShader = &ShaderManager::Getshaderbyname("m_LightingCube");
    HouseShader        = &ShaderManager::Getshaderbyname("m_House");
    HDRShader          = &ShaderManager::Getshaderbyname("m_HDR");
}
