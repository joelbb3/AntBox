#pragma once
#include <memory>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include "Entity.h"
#include "Affordance.h"
#include "stdafx.h"
class Vector;
class GridManager;
class Affordance;
class Agent;



enum AgentType {
	ant,
	food
};



struct AgentState {
	/*
     Define properties that agents need to track as part of their state here.
     Would like to find a way to avoid having to lump different agents' state data members together whilst maintaining polymorphism in the Agent definition and in affordances (right now affordances operate through a pointer to an AgentState; don't want to have to make a seperate version of an affordance for each type of agent).
     */
};



class AgentManager : Steppable {
private:
	GridManager& gridManager;
	static Vector defaultPosition;
	std::multimap<int, std::shared_ptr<Agent>> agentList; // Each agent is keyed by the ID# of the face they occupy.

	AgentManager(GridManager& gridManager, Vector defaultPosition);

	bool isColliding(const Agent& a1, const Agent& a2);
	void createAgent(AgentType type, Vector position = defaultPosition);
	void updateNeighbours();
	void step();
public:
	enum AgentManagerFlag {
		kill,
		moved,
		colliding
	};
};



class Agent : public Steppable {
    friend class AgentManager;
	friend class AffordanceFactory;
private:
    int maxNeighbourRadius;
    const AgentManager& manager;
    std::set<AgentManager::AgentManagerFlag> flags; // Flags for use by the agent manager.
	std::unique_ptr<AgentState> statepnt;	// Used to access state internally.
    std::vector<std::weak_ptr<const Agent>> neighbourList;
    std::vector<std::weak_ptr<const Agent>> collidingAgents;
    
    Agent(AgentManager& manager, std::string& texturePath, Vector startingPosition);
    
public:
	std::vector<Affordance> affordanceList;
};



class Ant : public Agent {
public:
	Ant();
};

class Food : public Agent {
public:
	Food();
};

class AgentFactory{
public:
	static std::shared_ptr<Agent> make(AgentType type);
};

