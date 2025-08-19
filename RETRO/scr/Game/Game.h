#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Renderer/Renderer.h"


enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
}; 

class Game
{
public:
        // game state
        GameState    State;	
        bool         Keys[1024];
        unsigned int Width, Height;
        // constructor/destructor
        Game(unsigned int width, unsigned int height);
        ~Game();
        // initialize game state (load all shaders/textures/levels)
        void Init();
        // game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
        void RenderLoadingScreen(float Progress);
};

