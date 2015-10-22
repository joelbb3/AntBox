#include "Agent.h"
#include "Grid.h"
#include <memory>

int Agent::agentIDGenerator = 0;

Agent::Agent(std::string& texturePath, Vector startingPosition) : Steppable(texturePath, startingPosition), agentID(agentIDGenerator++) {
}



