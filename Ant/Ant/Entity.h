#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "Geometry.h"
#include "stdafx.h"

class VisualObject {
public:
    Vector position;
    static int IDGenerator;
    const int ID;
    
    VisualObject(std::string& texturePath, Vector initialPosition);
    VisualObject();
    
	Box getBoundingBox() const { return Box(Vector(0, 0, 0), 0, 0); };
};



class Steppable : public VisualObject {
private:
    //sf::Clock stepClock;
    
public:
    Steppable(std::string texturePath, Vector initialPosition);
    
    virtual void step(){};
};


