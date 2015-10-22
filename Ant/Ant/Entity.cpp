#include "Agent.h"
#include "Grid.h"
#include <utility>
#include <string>

/* VisualObject -- still needs to be defined -- */

int VisualObject::visualObjectIDGenerator = 0;

VisualObject::VisualObject(std::string& texturePath, Vector initialPosition) : visualObjectID(visualObjectIDGenerator++), position(initialPosition){
    texture.loadFromFile(texturePath);
    texture.setSmooth(true);
    sprite.setTexture(texture);
}

VisualObject::VisualObject() : visualObjectID(visualObjectIDGenerator++){
};

/* Steppable */

Steppable::Steppable(std::string texturePath, Vector initialPosition) : VisualObject(texturePath, initialPosition) {
    stepClock.restart();
}


