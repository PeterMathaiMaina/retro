#ifndef GLOBALASSETS_H
#define GLOBALASSETS_H

#include "../CAMERA/Camera.h"

struct GlobalAssets {
    static Camera& GetCamera();

private:
    static Camera m_camera;
};

#endif
