#pragma once
#include <memory>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include "Entity.h"
#include "Affordance.h"

class Vector;
class GridManager;
class Affordance;
class AgentManager;

enum AgentType {
	ant
};

enum AgentFlags{
    kill,
    moved
};

class Agent : public Steppable {
private:
    static int agentIDGenerator;
    friend class AgentManager;
	//friend class AffordanceFactory;
protected:
    int agentID;
    int faceID;
    int maxNeighbourRadius;
    std::set<AgentFlags> flags; // Flags for use by the agent manager.
    std::vector<std::weak_ptr<const Agent>> neighbourList;
    std::vector<std::weak_ptr<const Agent>> collidingAgents;
    Agent(std::string& texturePath, Vector startingPosition);
    
public:
	//std::vector<Affordance> affordanceList;
};


