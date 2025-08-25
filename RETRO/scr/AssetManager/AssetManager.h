#pragma once
#include "GL/glew.h"
#include <map>
#include <string>
#include "../../third_party/imageprocessing/stb_image.h"
#include "../graphics/Model.h"
#include <vector>
#include <iostream>
#include "../../textureLoader/textureLoader.hpp"
#include "../Util/Util.h"



enum class AssetType {
    Texture,
    Model,
    Shader
};


struct AssetJob {
    AssetType type;
    std::string name;

    // for model / texture
    std::string path;  

    // for shader
    const char* vertexPath;
    const char* fragmentPath;
    const char* GeometryPath;
};


class AssetManager{
public:
    static std::vector<AssetJob> AssetsToLoad;
    static std::vector<std::string>LoadedModelTextures;
    static std::map<std::string,Model> Models;
    static std::map<std::string,Shader> Shaders;
    static std::map<std::string, GLuint> GPUTextureIDs;
    static std::vector<Texture> g_textures;
    static std::map<std::string, Texture> TexturesByPath;



    static void QueueAssets();
    static bool LoadingComplete();
    static void UpdateLoading();
    static float GetProgress();
    static void Init();
    static void FindAssetPaths();
    static unsigned int GetTextureIDbyname(const std::string& name);
    static unsigned int GetTextureIDbyPath(const std::string& path);
    static Model* GetModelbyname(const std::string& name);
    static GLuint GetOrLoadTexture(const std::string& path);
    ~AssetManager();
private:
 
    static size_t CurrentAssetIndex;
};