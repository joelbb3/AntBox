#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "Geometry.h"
#include "stdafx.h"

class GridManager;

class Agent;
class Affordance;

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



class AgentManager : Steppable {
private:
    GridManager& gridManager;
    static Vector defaultPosition;
    std::multimap<int, std::shared_ptr<Agent>> agentList; // Each agent is keyed by the ID# of the face they occupy.
    
    AgentManager(GridManager& gridManager, Vector defaultPosition);
    
    bool isColliding(const Agent& a1, const Agent& a2);
    void createAgent(std::string type, Vector position = defaultPosition);
    void updateNeighbours();
    void step();
public:
    enum AgentManagerFlag {
        kill,
        moved,
        colliding
    };
};