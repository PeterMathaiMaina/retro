#include "Game.h"
#include "../core/ShaderManager.hpp"
#include "../GlobalAssets/GlobalAssets.h"
#include "../AssetManager/AssetManager.h"
#include "GlobalAssets/GlobalAssets.h"
#include "../Input/Input.h"
#include "../BackEnd/BackEnd.h"



Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    
}

void Game::Init()
{
   AssetManager::Init();
    // Input::Init(BackEnd::GetWindow());
} 

void Game::Update(float dt)
{
    
}

void Game::ProcessInput(float dt)
{
    Input::ProcessKeyboard(GlobalAssets::GetCamera(),dt);
    GlobalAssets::GetCamera().ProcessMouseMovement(Input::GetMouseOffsetX(),Input::GetMouseOffsetY());
    GlobalAssets::GetCamera().Update();
    
}

void Game::Render()
{
    Renderer::Renderscene();
}
void Game::RenderLoadingScreen(float progress) {
    // Loading screen background = dark blue
    glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // For now, just draw a simple progress bar using immediate mode
    glBegin(GL_QUADS);
        glColor3f(0.2f, 0.8f, 0.3f);
        glVertex2f(-0.8f, -0.8f);
        glVertex2f(-0.8f + progress * 1.6f, -0.8f);
        glVertex2f(-0.8f + progress * 1.6f, -0.7f);
        glVertex2f(-0.8f, -0.7f);
    glEnd();
}
