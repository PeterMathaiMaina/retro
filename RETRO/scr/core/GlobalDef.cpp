#include "GlobalDef.h"
#include "ShaderManager.hpp"
#include "../AssetManager/AssetManager.h"

// Shader definitions
Shader* globals::HouseShader = nullptr;
Shader* globals::BunnyShader = nullptr;
Shader* globals::CubeShader = nullptr;
Shader* globals::HeightMapShader = nullptr;
Shader* globals::HDRShader = nullptr;
Shader* globals::LightingCubeShader = nullptr;
Shader* globals::TreeShader = nullptr;

// Model* globals::BunnyModel = nullptr;
// Model* globals::HouseModel = nullptr;
// Model* globals::TreeModel = nullptr;

void GlobalDef::GlobalInit() {
    ShaderManager::Init();
    // AssetManager::Init();

    // globals::HouseShader = &ShaderManager::Getshaderbyname("m_House");
    // globals::BunnyShader = &ShaderManager::Getshaderbyname("m_Bunny");
    // globals::CubeShader = &ShaderManager::Getshaderbyname("m_Cube");
    // globals::HeightMapShader = &ShaderManager::Getshaderbyname("m_HeightMap");
    // globals::HDRShader = &ShaderManager::Getshaderbyname("m_HDR");
    // globals::LightingCubeShader = &ShaderManager::Getshaderbyname("m_LightingCube");
    // globals::TreeShader = &ShaderManager::Getshaderbyname("m_Tree");

    // globals::BunnyModel = &AssetManager::GetModelByName("bunny");
    // globals::HouseModel = &AssetManager::GetModelByName("house");
    // globals::TreeModel = &AssetManager::GetModelByName("tree");
}
