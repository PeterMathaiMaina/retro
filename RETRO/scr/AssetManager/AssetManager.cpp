#include "AssetManager.h"
std::vector<AssetJob> AssetManager::AssetsToLoad;
std::vector<std::string> AssetManager::LoadedModelTextures;
std::map<std::string,GLuint> AssetManager::GPUTextureIDs;
std::vector<Texture> AssetManager::g_textures;
std::map<std::string, Texture> AssetManager::TexturesByPath;
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
        {AssetType::Model,   "BunnyModel", "/home/peter/retro/RETRO/rescources/models_raw/bunny/scene.gltf"},
        {AssetType::Model,   "HouseModel", "/home/peter/retro/RETRO/rescources/models_raw/House1/House.obj"},
        //RETRO/rescources/models_raw/House1/House.obj
        // {AssetType::Shader,  "DefaultShader","shaders/default.vert" },
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

GLuint AssetManager::GetOrLoadTexture(const std::string& path) {
    auto it = GPUTextureIDs.find(path);
    if (it != GPUTextureIDs.end()) {
        return it->second; // Already loaded
    }

    // Otherwise, load it
    GLuint id = TextureFromFile(path);
    GPUTextureIDs[path] = id;
    return id;
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
            for ( const auto& tex : model.textures_loaded ){
                auto it = std::find_if(g_textures.begin(), g_textures.end(),
                    [&](const Texture& t) { return t.path == tex.path; });
                if (it == g_textures.end()) {
                    g_textures.push_back(tex);
                }
            } 
            // std::cout << TexturesByPath.size() <<std::endl;
            break;
        }
        case AssetType::Shader:{
            break;
        }
    }
    CurrentAssetIndex++;
}

Model* AssetManager::GetTextureModelbyname(const std::string& name){
    auto it = Models.find(name);
    if(it!=Models.end()){
        return &it->second;
    }
    std::cerr<<"Model not found"<<name<<std::endl;
    return nullptr;
}


bool AssetManager::LoadingComplete() {
    return CurrentAssetIndex >= AssetsToLoad.size();
}

float AssetManager::GetProgress() {
    if (AssetsToLoad.empty()) return 1.0f;
    return (float)CurrentAssetIndex / (float)AssetsToLoad.size();
}