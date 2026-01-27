#include "GUI.hpp"
#include "DummyEngine.hpp"
#include <vector>

int main() {
    Scene scene;
    Track track;
    // crea 4 note di test
    for (int i = 0; i < 4; ++i)
        track.notes.push_back({i, i, 0.0f, true});
    scene.tracks.push_back(track);

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
