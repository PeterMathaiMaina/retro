#include "GlobalAssets.h"

Camera GlobalAssets::m_camera;  // definition of the static object

Camera& GlobalAssets::GetCamera() {
    return m_camera;
}
