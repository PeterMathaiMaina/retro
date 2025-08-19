#include "GL/glew.h"
#include <map>
#include <string>
#include "../../third_party/imageprocessing/stb_image.h"
#include "../graphics/Model.hpp"
#include <vector>
#include <iostream>
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


struct AssetManager{
    static void QueueAssets();
    static bool LoadingComplete();
    static void UpdateLoading();
    static float GetProgress();
    static void Init();
    static unsigned int GetTextureIDbyname(const std::string& name);
    ~AssetManager();
private:
 
    static std::vector<AssetJob> AssetsToLoad;
    static size_t CurrentAssetIndex;
    static std::map<std::string, Model> Models;
    static std::map<std::string, GLuint> GPUTextureIDs;
};