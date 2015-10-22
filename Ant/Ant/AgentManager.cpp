#include "AgentManager.h"
#include "Agent.h"
#include "Ant.h"
#include <iostream>

std::shared_ptr<sf::RenderWindow> AgentManager::window; // Reference to window for drawing agents to.
std::map<int, int> AgentManager::agentFaceMap; // Data structure hashing agent IDs to their face IDs.
std::map<AgentType, std::string> AgentManager::textureMap; // Data structure hashing each agent type to its texture path.
std::vector<std::shared_ptr<Agent>> AgentManager::agentList; // List containing agents.

void AgentManager::createAgent(AgentType type, Vector position) {
    std::cout << "AgentManager creating new agent.\n";
    switch(type){
        case AgentType::ant:
            // Make the agent.
            std::cout << "Instantiating...\n";
            std::shared_ptr<Agent> newAgent{new Ant(textureMap[AgentType::ant], position)};
            
            // Scale the texture appropriately.
            std::cout << "Scaling...\n";
            newAgent->sprite.scale(0.03, 0.03);
            
            // Update the sprite to reflect internal position.
            std::cout << "Setting position...\n";
            newAgent->sprite.setPosition(position.x, position.y);
            
            // Add to the agent list.
            std::cout << "Storing agent pointer...\n";
            agentList.push_back(newAgent);
            
            // Hash the agent ID to its associated face ID.
            std::cout << "Storing agent location...\n\n";
            agentFaceMap.insert(std::make_pair(newAgent->agentID, GridManager::locatePoint(newAgent->position)));
            
    }
    std::cout << "Success!\n";
}

void AgentManager::stepAgents() {
    std::cout << "Stepping agents...\n";
    for (auto it = agentList.begin(); it != agentList.end(); ) {
        std::cout << "Now stepping agent #" << it->get()->agentID << " (Face#: " << agentFaceMap[it->get()->agentID] << ", raw position: "; it->get()->position.print(); std::cout << ")\n";
        // Check if the agent is dead. If yes, erase and move on.
        if(it->get()->flags.count(AgentFlags::kill)){
            std::cout << "This agent is dead! Removing now...\n";
            it = agentList.erase(it);
            continue;
        }
        else{
            std::cout << "Calling internal step...\n\n";
            // Let the agent make its step.
            it->get()->step();
            
            // If the agent has moved, update its position in the face ID hashmap.
            if(it->get()->flags.count(AgentFlags::moved)){
                std::cout << "Agent has moved. Relocating...\n";
                agentFaceMap[it->get()->agentID] = GridManager::locatePoint(it->get()->position);
                std::cout << "Agent is now on face #: " << agentFaceMap[it->get()->agentID] << " with raw position: "; it->get()->position.print(); std::cout << "\n";
            }
            
            // Draw the agent.
            std::cout << "Drawing the agent...\n";
            window->draw(it->get()->sprite);
        }
        std::cout << "Stepping of agent # " << it->get()->agentID << " now complete!\n";
        ++it;
    }
    std::cout << "All stepping complete!\n";
    /*
    // Check for collisions.
    for (int i = agentList.begin()->first; i <= agentList.rbegin()->first; ++i) {
        // Only bother checking faces containing more than one agent.
        if (agentList.count(i) > 1) {
            std::pair<std::multimap<int, std::shared_ptr<Agent>>::iterator, std::multimap<int, std::shared_ptr<Agent>>::iterator> proximityRange = agentList.equal_range(i);
            for (auto it = proximityRange.first; it != proximityRange.second; ++it) {
                for (auto collider = proximityRange.first; collider != proximityRange.second; ++it) {
                    if (it == collider) {
                        continue;
                    }
                    if (isColliding(*it->second, *collider->second)) {
                        it->second->flags.insert(AgentManagerFlag::colliding);
                        it->second->collidingAgents.push_back(collider->second);
                    }
                }
            }
        }
    }
     */
    // Update neighbour lists.
    //updateNeighbours();
}

void AgentManager::setTexture(AgentType type, std::string path){
    std::cout << "Setting a new texture in the AgentManager...\n";
    if(textureMap.count(type)){
        textureMap[type] = path;
    }else{
        textureMap.insert(std::make_pair(type, path));
    }
    std::cout << "Texture set!\n";
}

void AgentManager::setWindow(sf::RenderWindow& window){
    std::cout << "Setting a new window in the AgentManager...\n";
    sf::RenderWindow* pnt = &window;
    AgentManager::window = std::shared_ptr<sf::RenderWindow>{pnt};
    std::cout << "Window set!\n";
}








/*
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
 */









/*
bool AgentManager::isColliding(const Agent& a1, const Agent& a2) {
    Box a1Box = a1.getBoundingBox();
    Box a2Box = a2.getBoundingBox();
    std::vector<Vector> a1Points = { a1Box.topLeft, a1Box.topRight, a1Box.bottomLeft, a1Box.bottomRight };
    std::vector<Vector> a2Points = { a2Box.topLeft, a2Box.topRight, a2Box.bottomLeft, a2Box.bottomRight };
    for (auto& x : a1Points) {
        if (a2Box.contains(x)) {
            return true;
        }
    }
    for (auto& x : a2Points) {
        if (a1Box.contains(x)) {
            return true;
        }
    }
    return false;
}*/
