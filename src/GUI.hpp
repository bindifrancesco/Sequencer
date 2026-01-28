/*GUI.hpp*/

#pragma once
#include <SDL2/SDL.h>
#include "DummyEngine.hpp"
#include <functional>
#include <map>


class GUI {
public:
    GUI(int width, int height);
    ~GUI();
    void drawScene(const Scene& scene, int cursorStep);
    void handleEvent(SDL_Event& e, int& cursorStep, Scene& scene);
    void drawGridForTrack(const Track& track, int yStart, int yEnd);


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
    std::map<SDL_Keycode, std::function<void()>> keyBindings;
};



