#include "GlobalFunctions.h"

// Example global screen vars
static int SCR_WIDTH = 1780;
static int SCR_HEIGHT = 1000;
static int SHADOW_WIDTH = 2048;
static int SHADOW_HEIGHT = 2048;

int GetscrWIDTH() { return SCR_WIDTH; }
int GetscrHEIGHT() { return SCR_HEIGHT; }
int GetsdhWIDTH() { return SHADOW_WIDTH; }
int GetsdhHEIGHT() { return SHADOW_HEIGHT; }

glm::mat4 GetProjectionMat() {
    float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
    return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

