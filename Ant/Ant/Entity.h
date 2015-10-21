#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "Geometry.h"

class VisualObject {
private:
    static int visualObjectIDGenerator;
public:
    const int visualObjectID;
    Vector position;
    sf::Sprite sprite;
    sf::Texture texture;
    VisualObject(std::string& texturePath, Vector initialPosition);
    VisualObject();
	//Box getBoundingBox() const { return Box(Vector(0, 0, 0), 0, 0); };
};



class Steppable : public VisualObject {
protected:
    sf::Clock stepClock;
    
public:
    Steppable(std::string texturePath, Vector initialPosition);
    virtual void step(){};
};


