#include "Renderer.h"
#include "../GlobalAssets/GlobalAssets.h"


void Renderer::Renderscene(){
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    Shader TreeShader = ShaderManager::Getshaderbyname("g_TreeShader");
    TreeShader.setMat4("model",glm::mat4(1.0f));
    TreeShader.setMat4("view",GlobalAssets::GetCamera().GetViewMatrix());
    TreeShader.setMat4("projection",glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f,0.1f,100.0f));//GetProjectionMat()


    Model* Tree = AssetManager::GetModelbyname("TreeModel");
    Tree->Draw(TreeShader);
    // std::cout << "in the main render loop\n";

}