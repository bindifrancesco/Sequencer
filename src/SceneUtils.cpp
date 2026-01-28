/*SceneUtils.cpp*/

#include "SceneUtils.hpp"
#include <iostream>  // opzionale, serve solo se fai debug/print

void addTrackToScene(Scene& scene, int numerator, int denominator) {
    Track t;
    t.signature = {numerator, denominator};

    // Calcolo automatico della gridSubdivision
    // Per semplicità: subdivision = numeratore
    t.gridSubdivision = numerator;
    t.notes.clear();
    scene.addTrack(t);
}
