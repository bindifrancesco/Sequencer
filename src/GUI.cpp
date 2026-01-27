/*GUI.cpp*/

#include "GUI.hpp"
#include <iostream>


GUI::GUI(int w, int h) : width(w), height(h),
                         windowedWidth(w), windowedHeight(h) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "P3MidiGUI",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_RESIZABLE   // consigliato
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


GUI::~GUI() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GUI::drawScene(const Scene& scene, int cursorStep) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    //velocita' cursore
    //bug faceva muovere riga sempre senza prendere start, stop
    //cursorX += 2.0f;   

    //se esce dallo schermo, riparte da sinistra
    if (cursorRunning) {
        cursorX += cursorSpeed;
        if (cursorX > width)
            cursorX = 0.0f;
    }

    int stepWidth = width / 16; // 16 step per default
    int pitchHeight = height / 12; // 12 note range fittizia

    if(drawNotes){
        for (size_t t = 0; t < scene.tracks.size(); ++t) {
            for (const auto& note : scene.tracks[t].notes) {
                if (!note.active) continue;
                SDL_Rect rect;
                rect.x = note.step * stepWidth + static_cast<int>(note.offset * stepWidth);
                rect.y = height - (note.pitch+1)*pitchHeight;
                rect.w = stepWidth - 2;
                rect.h = pitchHeight - 2;
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // disegno cursore
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);

    //SDL_Rect cursorRect = { cursorStep * stepWidth, 0, stepWidth, height };  //prima
    //int cursorX = cursorStep * stepWidth; //dopo (ancora statico)
    int drawX = static_cast<int>(cursorX);

    SDL_Rect cursorRect = {
        drawX,
        0,
        2,      // larghezza della riga rossa (2 pixel)
        height
    };
    SDL_RenderFillRect(renderer, &cursorRect);

    SDL_RenderPresent(renderer);
}

void GUI::handleEvent(SDL_Event& e, int& cursorStep, Scene& scene) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_s: //start/stop
                cursorRunning = !cursorRunning;
                break;
            case SDLK_j: //rallenta
                cursorSpeed -= 0.5f;
                if (cursorSpeed < 0.1f) cursorSpeed = 0.1f; //limite minimo
                break;
            case SDLK_k: //rallenta
                cursorSpeed += 0.5f;
                if (cursorSpeed < 0.1f) cursorSpeed = 0.1f; //limite minimo
                break;
            case SDLK_n:
                drawNotes = !drawNotes;
                break;
            case SDLK_f:
                fullScreen = !fullScreen;
                if (fullScreen) {
                    // salva size attuale prima di andare fullscreen
                    SDL_GetWindowSize(window, &windowedWidth, &windowedHeight);
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                } else {
                    SDL_SetWindowFullscreen(window, 0);
                    // ripristina size precedente
                    SDL_SetWindowSize(window, windowedWidth, windowedHeight);
                }
                // aggiorna width/height usate dal rendering
                SDL_GetWindowSize(window, &width, &height);
                break;
        }
        switch (e.key.keysym.sym) {
            case SDLK_RIGHT: cursorStep = (cursorStep + 1) % 16; break;
            case SDLK_LEFT:  cursorStep = (cursorStep + 15) % 16; break;
            case SDLK_RETURN: // attiva/disattiva nota sulla track 0, pitch 0
                if (!scene.tracks.empty() && !scene.tracks[0].notes.empty())
                    scene.tracks[0].notes[0].active = !scene.tracks[0].notes[0].active;
                break;
        }
    }
}
