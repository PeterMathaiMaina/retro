#include "AssetManager.h"


std::map<std::string,Model> AssetManager::Models;
std::map<std::string,unsigned int> AssetManager::Textures;

Model& AssetManager::GetModelByName(const std::string& name) {
    if(Models.count(name)>0)
        return Models.find(name)->second;
    auto it = Models.find(name);
    if (it != Models.end()) {
        return it->second;
    } else {
        std::cerr << "[AssetManager] Error: Model '" << name << "' not found!" << std::endl;
        throw std::runtime_error("Model not found");
    }
}

unsigned int AssetManager::GetTextureByName(const std::string& name) {
    auto it = Textures.find(name);
    if (it != Textures.end()) {
        return it->second;
    } else {
        std::cerr << "[AssetManager] Error: Texture '" << name << "' not found!" << std::endl;
        throw std::runtime_error("Texture not found");
    }
}

void AssetManager::Init() {
    std::cout << "[AssetManager] Initializing assets..." << std::endl;

    // Load Textures
    Textures["Specular"] = TextureFromFile("Specular.jpe","/home/peter/retro/RETRO/rescources/textures/Uncompressed");
    Textures["bricks"] =  TextureFromFile("bricks2.jpg","/home/peter/retro/RETRO/rescources/textures/Uncompressed");
    Textures["bricksNRM"] =   TextureFromFile("bricks2_normal.jpg", "/home/peter/retro/RETRO/rescources/textures/Uncompressed");

    // Load Models
    Models.emplace("bunny", Model("/home/peter/retro/RETRO/rescources/models_raw/bunny/scene.gltf"));
    Models.emplace("house", Model("/home/peter/retro/RETRO/rescources/models_raw/House1/House.obj"));
    Models.emplace("tree",  Model("/home/peter/retro/RETRO/rescources/models_raw/tree/TreeLarge_0.obj"));

    std::cout << "[AssetManager] Assets loaded successfully." << std::endl;
}

