#pragma once

#include "Shader.hpp"
#include "../graphics/Model.h"

namespace globals {
    extern Shader* HouseShader;
    extern Shader* BunnyShader;
    extern Shader* CubeShader;
    extern Shader* HeightMapShader;
    extern Shader* HDRShader;
    extern Shader* LightingCubeShader;
    extern Shader* TreeShader;

    extern Model* BunnyModel;
    extern Model* HouseModel;
    extern Model* TreeModel;
}


class GlobalDef {
public:
    static void GlobalInit();
};
