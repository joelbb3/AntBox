#include "stdafx.h"
#include "Agent.h"
#include "Entity.h"
#include <utility>
#include <string>
#include <SFML/Graphics.hpp>

int VisualObject::IDGenerator = 0;

VisualObject::VisualObject(std::string& texturePath, Vector initialPosition) : ID(IDGenerator++), position(initialPosition) {
	texture = std::unique_ptr<sf::Texture>(new sf::Texture);
	texture->loadFromFile(texturePath);
}

Steppable::Steppable(std::string& texturePath, Vector initialPosition) : VisualObject(texturePath, initialPosition) {
	stepClock.restart();
}


void AgentManager::updateNeighbours() {
	for (auto it = agentList.begin(); it != agentList.end(); ++it) {		// For every agent in our list...
		std::vector<int> neighbouringFaces = GridManager::getNeighbours(it->first);		// Use its associated face ID to get a list of neighbouring faces.
		std::vector<const Agent*> neighbouringAgents;
		for (auto neighbour : neighbouringFaces) {		// Then for every neighbouring face, search for any agent on that face (using its ID), and add a reference to that agent to the list.
			std::pair<std::multimap<int, std::shared_ptr<Agent>>::iterator, std::multimap<int, std::shared_ptr<Agent>>::iterator> neighbourRange;
			neighbourRange = agentList.equal_range(neighbour);
			for (auto rangeit = neighbourRange.first; rangeit != neighbourRange.second; ++rangeit) {
				neighbouringAgents.push_back(rangeit->second.get());
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