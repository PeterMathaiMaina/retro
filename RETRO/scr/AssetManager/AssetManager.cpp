#include "AssetManager.h"
std::vector<AssetJob> AssetManager::AssetsToLoad;
std::vector<std::string> AssetManager::LoadedModelTextures;
std::map<std::string,GLuint> AssetManager::GPUTextureIDs;
// std::map<std::string,GLuint> AssetManager::GPUTextureIDsByPath;
std::map<std::string,Model> AssetManager::Models;
size_t AssetManager::CurrentAssetIndex = 0;
void AssetManager::Init(){
    AssetManager::QueueAssets();
}

void AssetManager::QueueAssets() {
    AssetsToLoad = {
        {AssetType::Texture, "BrickTex", "/home/peter/retro/RETRO/rescources/textures/Uncompressed/HEIGHTMAP.png"},
        {AssetType::Texture, "ContainerTex", "/home/peter/retro/RETRO/rescources/textures/Compressed/container.dds"},
        {AssetType::Model,   "TreeModel", "/home/peter/retro/RETRO/rescources/models_raw/tree/TreeLarge_0.obj"},
        {AssetType::Shader,  "DefaultShader","shaders/default.vert" },
    };
    CurrentAssetIndex = 0;
}
unsigned int AssetManager::GetTextureIDbyname(const std::string& name){

    auto it = GPUTextureIDs.find(name);
    if (it !=GPUTextureIDs.end()){
        return it->second;
    }
    std::cerr << "[ASSETMANAGER] could not find the texture"<<std::endl;
    return 0;
}


void AssetManager::UpdateLoading() {
    if (CurrentAssetIndex >= AssetsToLoad.size()) return;
    // std::cout<<AssetsToLoad.size()<<std::endl;

    const AssetJob& job = AssetsToLoad[CurrentAssetIndex];

    switch (job.type) {
        case AssetType::Texture: {
            unsigned int TextureID = TextureFromFile(job.path); // stb_image wrapper
            GPUTextureIDs[job.name] = TextureID;
            break;
        }
        case AssetType::Model: {
            Model model(job.path);
            Models[job.name] = model;
            break;
        }
        case AssetType::Shader:{
            break;
        }
    }
    CurrentAssetIndex++;
}


bool AssetManager::LoadingComplete() {
    return CurrentAssetIndex >= AssetsToLoad.size();
}

float AssetManager::GetProgress() {
    if (AssetsToLoad.empty()) return 1.0f;
    return (float)CurrentAssetIndex / (float)AssetsToLoad.size();
}