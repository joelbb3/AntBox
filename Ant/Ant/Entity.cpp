#include "stdafx.h"
#include "Entity.h"
#include <utility>
#include "Agent.h"

void AgentManager::updateNeighbours() {
	for (auto it = agentList.begin(); it != agentList.end(); ++it) {		// For every agent in our list...
		std::vector<int> neighbouringFaces = GridManager::getNeighbours(it->first);		// Use its associated face ID to get a list of neighbouring faces.
		std::vector<const Agent*> neighbouringAgents;
		for (auto neighbour : neighbouringFaces) {		// Then for every neighbouring face, search for any agent on that face (using its ID), and add a reference to that agent to the list.
			std::pair<std::multimap<int, Agent*>::iterator, std::multimap<int, Agent*>::iterator> neighbourRange;
			neighbourRange = agentList.equal_range(neighbour);
			for (auto rangeit = neighbourRange.first; rangeit != neighbourRange.second; ++rangeit) {
				neighbouringAgents.push_back(rangeit->second);
			}
		}
		//it->second->updateNeighbours(neighbouringAgents);
	}
}



/*
void AgentManager::addAgent(int face, AgentType toAdd) {
	Agent* newAgent;
	switch (toAdd) {
	case ant:
		newAgent = new Ant();
	case food:
		newAgent = new Food();
	}
	agentList.insert(std::make_pair(face, newAgent));
}

*/