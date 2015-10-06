#include "Agent.h"
#include <memory>

/* Agent */

Agent::Agent(AgentManager& manager, std::string& texturePath, Vector startingPosition) : manager(manager), Steppable(texturePath, startingPosition) {
	statepnt = std::unique_ptr<AgentState>(new AgentState);
}

