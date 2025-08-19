#include "Renderer.h"


void Renderer::Renderscene(){
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_MULTISAMPLE);
    glClearColor(1.0f, 1.1f, 1.0f, 1.0f);
    std::cout << "in the main render loop\n";

}