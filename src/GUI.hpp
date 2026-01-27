/*GUI.hpp*/

#pragma once
#include <SDL2/SDL.h>
#include "DummyEngine.hpp"

class GUI {
public:
    GUI(int width, int height);
    ~GUI();
    void drawScene(const Scene& scene, int cursorStep);
    void handleEvent(SDL_Event& e, int& cursorStep, Scene& scene);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width, height;
    float cursorX = 0.0f;
    bool cursorRunning = true;
    float cursorSpeed = 2.0f;
    bool drawNotes = false;
    bool fullScreen = false;
    int windowedWidth  = 900;
    int windowedHeight = 500;
};



