#pragma once
#include "stdafx.h"
#include <vector>
#include "Geometry.h"
#include <memory>
#include <map>
#include "Grid.h"
#include "SFML\Graphics.hpp"

class Agent;
class Affordance;
enum AgentType;

class VisualObject {
private:
    Vector position;
	std::unique_ptr<sf::Texture> texture;
    std::unique_ptr<sf::Sprite> sprite;
    
public:
    static int IDGenerator;
    const int ID;
    
	VisualObject(std::string& texturePath, Vector initialPosition);
    VisualObject();
    
	Box getBoundingBox() const;
};



class Steppable : public VisualObject {
private:
	sf::Clock stepClock;
    
public:
	Steppable(std::string& texturePath, Vector initialPosition);
    
	virtual void step(){};
};



class AgentManager : Steppable {
private:
    enum AgentManagerFlags{
        kill,
        moved
    };
    
    GridManager& gridManager;
    Vector defaultPosition;
    std::multimap<int, std::shared_ptr<Agent>> agentList; // Each agent is keyed by the ID# of the face they occupy.
    
    AgentManager(GridManager& gridManager, Vector defaultPosition);
    
    bool isColliding(const Agent& a1, const Agent& a2);
    void createAgent(Vector position);
	void updateNeighbours();
	void step();
};