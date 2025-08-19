#pragma once
#include "../core/ShaderManager.hpp"
#include "../graphics/Model.hpp"
#include <GL/glew.h>

class GlobalAssets {
public:
    static void Init();
    static void Shutdown();

    // Declarations only (no = nullptr here!)
    static Shader* CubeShader;
    static Shader* TreeShader;
    static Shader* BunnyShader;
    static Shader* HeightMapShader;
    static Shader* LightingCubeShader;
    static Shader* HouseShader;
    static Shader* HDRShader;
};
