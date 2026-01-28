/* GUI.cpp */

#include "GUI.hpp"
#include "SceneUtils.hpp"
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
        SDL_WINDOW_RESIZABLE
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // --- keybindings dinamici ---
    keyBindings[SDLK_s] = [this]() { cursorRunning = !cursorRunning; };
    keyBindings[SDLK_j] = [this]() { 
        cursorSpeed -= 0.5f; 
        if (cursorSpeed < 0.1f) cursorSpeed = 0.1f;
    };
    keyBindings[SDLK_k] = [this]() { 
        cursorSpeed += 0.5f;
        if (cursorSpeed < 0.1f) cursorSpeed = 0.1f;
    };
    keyBindings[SDLK_n] = [this]() { drawNotes = !drawNotes; };
    keyBindings[SDLK_f] = [this]() { 
        fullScreen = !fullScreen;
        if (fullScreen) {
            SDL_GetWindowSize(window, &windowedWidth, &windowedHeight);
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        } else {
            SDL_SetWindowFullscreen(window, 0);
            SDL_SetWindowSize(window, windowedWidth, windowedHeight);
        }
        SDL_GetWindowSize(window, &width, &height);
    };
}

GUI::~GUI() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void GUI::drawScene(const Scene& scene, int cursorStep) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    // Aggiorna posizione cursore
    if (cursorRunning) {
        cursorX += cursorSpeed;
        if (cursorX > width)
            cursorX = 0.0f;
    }

    int stepWidth = width / 16; // 16 step per default

    int trackCount = scene.tracks.size();
    if (trackCount == 0) return; // nessuna traccia

    int trackHeight = height / trackCount;

    // disegno tracce e griglia
    for (int t = 0; t < trackCount; ++t) {
        int yStart = t * trackHeight;
        int yEnd   = yStart + trackHeight;

        drawGridForTrack(scene.tracks[t], yStart, yEnd);

        // disegna note
        if (drawNotes) {
            int pitchCount = 12; // numero di pitch per traccia
            int pitchHeight = trackHeight / pitchCount;

            for (const auto& note : scene.tracks[t].notes) {
                if (!note.active) continue;

                SDL_Rect rect;
                rect.x = note.step * stepWidth + static_cast<int>(note.offset * stepWidth);
                rect.y = yEnd - (note.pitch + 1) * pitchHeight; // invertito y
                rect.w = stepWidth - 2;
                rect.h = pitchHeight - 2;

                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // disegno cursore rosso
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_Rect cursorRect = {
        static_cast<int>(cursorX),
        0,
        2,      // larghezza del cursore
        height
    };
    SDL_RenderFillRect(renderer, &cursorRect);

    SDL_RenderPresent(renderer);
}


void GUI::handleEvent(SDL_Event& e, int& cursorStep, Scene& scene) {
    
    std::cout << "Key pressed: " << e.key.keysym.sym << "\n";
    if (e.type == SDL_KEYDOWN) {

        // --- keybindings dinamici ---
        auto it = keyBindings.find(e.key.keysym.sym);
        if (it != keyBindings.end()) {
            it->second(); // esegue la funzione associata al tasto
        }

        // --- aggiungi traccia con tasto + ---
        if (e.key.keysym.sym == SDLK_KP_PLUS || 
            e.key.keysym.sym == SDLK_PLUS || 
            (e.key.keysym.sym == SDLK_EQUALS && (e.key.keysym.mod & KMOD_SHIFT))) 
        {
            addTrackToScene(scene, 4, 4);
            std::cout << "Nuova traccia aggiunta! Totale tracce: " << scene.tracks.size() << "\n";
        }

        // frecce e RETURN rimangono hardcoded
        switch (e.key.keysym.sym) {
            case SDLK_RIGHT: cursorStep = (cursorStep + 1) % 16; break;
            case SDLK_LEFT:  cursorStep = (cursorStep + 15) % 16; break;
            case SDLK_RETURN:
                if (!scene.tracks.empty() && !scene.tracks[0].notes.empty())
                    scene.tracks[0].notes[0].active = !scene.tracks[0].notes[0].active;
                break;
        }
    }
}


void GUI::drawGridForTrack(const Track& track, int yStart, int yEnd) {
    // Altezza della traccia
    int trackHeight = yEnd - yStart;

    // Numero totale di step della traccia
    // Calcoliamo la durata di una battuta in tick
    int ppq = 480; // puoi anche leggere da scene.ppq se lo passi
    int barTicks = (ppq * track.signature.numerator * 4) / track.signature.denominator;

    // Calcolo passo della griglia in tick
    int stepTick = barTicks / track.gridSubdivision;

    // Linee verticali
    for (int tick = 0; tick <= barTicks; tick += stepTick) {
        // calcola la posizione X in pixel
        int x = static_cast<int>((float(tick) / barTicks) * width); // width = larghezza della finestra

        // Scegli colore in base al tipo di linea
        if (tick % barTicks == 0) {           // inizio battuta
            SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        } else if (tick % (ppq) == 0) {      // beat
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        } else {                              // subdivision
            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
        }

        SDL_RenderDrawLine(renderer, x, yStart, x, yEnd);
    }

    // Linea orizzontale superiore/inferiore della traccia
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawLine(renderer, 0, yStart, width, yStart);
    SDL_RenderDrawLine(renderer, 0, yEnd, width, yEnd);
}

