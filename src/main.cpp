#include "GUI.hpp"
#include "DummyEngine.hpp"
#include "SceneUtils.hpp"
#include <vector>

int main() {
    Scene scene;
    Track track;
    scene.lengthTicks = 480 * 16; // 16 beat
    scene.ppq = 480;

    // Aggiungi tracce usando la funzione helper
    addTrackToScene(scene, 4, 4); // Track 0, 4/4
    addTrackToScene(scene, 6, 8); // Track 1, 6/8


    GUI gui(800, 400);
    bool quit = false;
    SDL_Event e;
    int cursorStep = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            gui.handleEvent(e, cursorStep, scene);
        }
        gui.drawScene(scene, cursorStep);
        SDL_Delay(50); // ~20 FPS
    }
    return 0;
}
