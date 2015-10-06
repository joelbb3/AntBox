#include "stdafx.h"
#include "Agent.h"
#include "Entity.h"
#include <utility>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

/* VisualObject -- still needs to be defined -- */

int VisualObject::IDGenerator = 0;

VisualObject::VisualObject(std::string& texturePath, Vector initialPosition) : ID(IDGenerator++), position(initialPosition) {
	texture = std::unique_ptr<sf::Texture>(new sf::Texture);
	texture->loadFromFile(texturePath);
}

VisualObject::VisualObject() : ID(IDGenerator++){
};

/* Steppable */

Steppable::Steppable(std::string& texturePath, Vector initialPosition) : VisualObject(texturePath, initialPosition) {
	stepClock.restart();
}


/* AgentManager */

AgentManager::AgentManager(GridManager& gridManager, Vector defaultPosition) : gridManager(gridManager), defaultPosition(defaultPosition){
};

void AgentManager::createAgent(AgentType type, Vector position = defaultPosition){
    agentList.insert(std::make_pair(GridManager::locatePoint(position), AgentFactory.make(type)));
}

void AgentManager::updateNeighbours() {
	for (auto it = agentList.begin(); it != agentList.end(); ++it) {		// For every agent in our list...
		std::vector<int> neighbouringFaces = GridManager::getNeighbours(it->first, it->second->maxNeighbourRadius);		// Use its associated face ID to get a list of neighbouring faces.
        std::vector<std::weak_ptr<const Agent>> neighbouringAgents;
		for (auto neighbour : neighbouringFaces) {		// Then for every neighbouring face, search for any agent on that face (using its ID), and add a reference to that agent to the list.
			std::pair<std::multimap<int, std::shared_ptr<Agent>>::iterator, std::multimap<int, std::shared_ptr<Agent>>::iterator> neighbourRange = agentList.equal_range(neighbour);
			for (auto rangeit = neighbourRange.first; rangeit != neighbourRange.second; ++rangeit) {
				neighbouringAgents.push_back(rangeit->second);
			}
		}
        it->second->neighbourList = neighbouringAgents;
	}
}

void AgentManager::step(){
    // Handle manager flags.
    for(auto it = agentList.begin(); it != agentList.end(); ++it){
        // Kill agents with kill flags.
        if(it->flags.count(AgentManagerFlag::kill)){
            it = agentList.erase(it);
        }
        // For any agents that have moved, update their associated face ID in the agent list.
        else if(it->flags.count(AgentManagerFlag::moved)){
            it->first = GridManager::locatePoint(it->second->position);
            it->flags.erase(AgentManagerFlag::moved);
        }
    }
    // Check for collisions.
    for(int i = *agentList.begin()->first; i <= *agentList.rbegin()->first; ++i){
        // Only bother checking faces containing more than one agent.
        if(agentList.count(i) > 1){
            std::pair<std::multimap<int, std::shared_ptr<Agent>>::iterator, std::multimap<int, std::shared_ptr<Agent>>::iterator> proximityRange = agentList.equal_range(neighbour);
            for(auto it = proximityRange.first; it != proximityRange.second; ++it){
                for(auto collider = proximityRange.first; collider != proximityRange.second; ++it){
                    if(it == collider){
                        continue;
                    }
                    if(isColliding(*it->second, *collider->second)){
                            it->second->flags.colliding = true;
                            it->second->collidingAgents.push_back(collider->second);
                    }
                }
            }
        }
    }
    // Update neighbour lists.
    updateNeighbours();
}

bool AgentManager::isColliding(const Agent& a1, const Agent& a2){
    std::vector<Vector> a1Points = {a1.topLeft, a1.topRight, a1.bottomLeft, a1.bottomRight};
    std::vector<Vector> a2Points = {a2.topLeft, a2.topRight, a2.bottomLeft,, a2.bottomRight};
    for(auto& x : a1Points){
        if(a2.getBoundingBox.contains(x)){
            return true;
        }
    }
    for(auto& x : a2Points){
        if(a1.getBoundingBox.contains(x)){
            return true;
        }
    }
    return false;
}