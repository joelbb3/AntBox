#include "stdafx.h"
#include "Agent.h"
#include "Grid.h"
#include <utility>
#include <string>

/* VisualObject -- still needs to be defined -- */

int VisualObject::IDGenerator = 0;

VisualObject::VisualObject(std::string& texturePath, Vector initialPosition) : ID(IDGenerator++), position(initialPosition) {
}

VisualObject::VisualObject() : ID(IDGenerator++){
};

/* Steppable */

Steppable::Steppable(std::string texturePath, Vector initialPosition) : VisualObject(texturePath, initialPosition) {
    //stepClock.restart();
}


