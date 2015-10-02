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
	std::unique_ptr<sf::Texture> texture;
	Vector position;
	std::vector<const VisualObject*> neighbourList;
public:
	VisualObject(std::string& texturePath, Vector initialPosition);
	VisualObject() : ID(5){};
	Box getBoundingBox() const {};
	static int IDGenerator;
	const int ID;
};

class Steppable : public VisualObject {
private:
	sf::Clock stepClock;
public:
	Steppable(std::string& texturePath, Vector initialPosition);
	virtual void step() {};
};

class AgentManager : Steppable {
private:
	std::vector<std::shared_ptr<Agent>> x;
	GridManager& gridManager;
	std::multimap<int, std::shared_ptr<Agent>> agentList;
	void updateNeighbours();
	void step();
};