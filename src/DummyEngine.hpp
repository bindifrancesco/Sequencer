/*DummyEngine.hpp*/

#pragma once
#include <vector>

struct NoteEvent {
    int pitch;
    int step;
    float offset; // microtiming frazione di step
    bool active;
};

struct TimeSignature {
    int numerator; //es. 6
    int denominator;//es. 8  => 6/8
};

struct Track {
    std::vector<NoteEvent> notes;
    TimeSignature signature;
    int gridSubdivision; //suddivisioni visive della battuta 
};

struct Scene {
    int lengthTicks; //durata totale della scena influenza la scala ritmica 
    int ppq;//pulses per quarter note, risoluzione temporale
    std::vector<Track> tracks;

    void addTrack(const Track& t){
        tracks.push_back(t);
    }
};
