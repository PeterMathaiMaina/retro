#pragma once
#include "GL/glew.h"
#include <map>
#include <string>
#include "../../third_party/imageprocessing/stb_image.h"
#include "../graphics/Model.h"
#include <vector>
#include <iostream>
#include "../../textureLoader/textureLoader.hpp"

struct CPUTexture {
    int width = 0;
    int height = 0;
    int channels = 0;                 // 1=G, 2=GA, 3=RGB, 4=RGBA
    std::vector<unsigned char> pixels; // size = width*height*channels
};
enum class AssetType {
    Texture,
    Model,
    Shader
};

struct AssetJob {
    AssetType type;
    std::string name;   // key used in maps
    std::string path;   // file path
};


class AssetManager{
public:
    static std::vector<AssetJob> AssetsToLoad;
    static std::vector<std::string>LoadedModelTextures;
    static std::map<std::string, GLuint> GPUTextureIDs;


    static void QueueAssets();
    static bool LoadingComplete();
    static void UpdateLoading();
    static float GetProgress();
    static void Init();
    static unsigned int GetTextureIDbyname(const std::string& name);
    static unsigned int GetTextureIDbyPath(const std::string& path);
    ~AssetManager();
private:
 
    static size_t CurrentAssetIndex;
    static std::map<std::string,Model> Models;
};