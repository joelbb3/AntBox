#pragma once
#include "stdafx.h"
#include "Entity.h"
#include <memory>
#include <vector>

class Vector; // three dimensional Vector from Geometry.h
class Affordance;

enum AgentType {
	ant,
	food
};

struct AgentState {
	/*---*/
};

class Agent : public Steppable {
	friend class AgentManager;
	friend class AffordanceFactory;
private:
	std::unique_ptr<AgentState> statepnt;	// Used to access state internally.
	AgentManager* manager;	// Used to interact with the world / other entities through the manager.
	Agent(AgentManager* manager);


	std::vector<const Agent*> neighbourList;
	//std::vector<const Affordance*> neighbourAffordanceList;
	void updateNeighbours(std::vector<const Agent*> neighbours);

public:
	//std::vector<Affordance> affordanceList;
};
