#pragma once
#include <map>
#include "Geometry.h"
#include "Grid.h"
#include "Entity.h"
#include "Agent.h"
#include <SFML/Graphics.hpp>

class AgentManager{
private:
    static std::shared_ptr<sf::RenderWindow> window; // Pointer to window for drawing agents to.
    static std::map<int, int> agentFaceMap; // Data structure hashing agent IDs to their face IDs.
    static std::map<AgentType, std::string> textureMap; // Data structure hashing each agent type to its texture path.
    static std::vector<std::shared_ptr<Agent>> agentList; // List containing agents.
    
    //void updateNeighbours();
    // bool isColliding(const Agent& a1, const Agent& a2);
public:
    static void setTexture(AgentType type, std::string path);
    static void setWindow(sf::RenderWindow& window);
    static void stepAgents();
    static void createAgent(AgentType type, Vector position);
    static int getFace(int agentID){
        return agentFaceMap[agentID];
    }
};

