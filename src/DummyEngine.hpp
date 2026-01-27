/*DummyEngine.hpp*/

#pragma once
#include <vector>

struct NoteEvent {
    int pitch;
    int step;
    float offset; // microtiming frazione di step
    bool active;
};

struct Track {
    std::vector<NoteEvent> notes;
};

struct Scene {
    std::vector<Track> tracks;
};
