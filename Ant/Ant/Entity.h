#pragma once
#include "stdafx.h"
#include <vector>
#include "Geometry.h"
#include <map>
#include "Grid.h"

namespace sf {
	class Texture;
	class Clock;
}
class Agent;
class Affordance;
enum AgentType;

class VisualObject {
private:
	static int IDGenerator;
	sf::Texture* texture;
	Vector position;
	std::vector<const VisualObject*> neighbourList;
public:
	VisualObject() : ID(0){};
	Box getBoundingBox() const {};
	const int ID;
};

class Steppable : public VisualObject {
private:
	sf::Clock* stepClock;
public:
	Steppable(){};
	virtual void step() {};
};

class AgentManager : Steppable {
private:
	std::map<int, Agent*> agentList;
	void updateNeighbours();
	std::vector<Affordance*> getAffordances(Agent* target) const;
	//void addAgent(int face, AgentType typeToAdd);
	void step();
};