#include "stdafx.h"
#include "Agent.h"
#include <memory>

Agent::Agent(AgentManager* manager) : manager(manager) {
	statepnt = std::unique_ptr<AgentState>(new AgentState);
}

void Agent::updateNeighbours(std::vector<const Agent*> neighbours) {
	neighbourList = neighbours;
}