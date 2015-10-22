//------------------------------------------------------------------------------------

// Includes and forward declarations.
#include "Affordance.h"
#include "AgentManager.h"
#include "Agent.h"
#include "Entity.h"
#include "Ant.h"
#include "Geometry.h"
#include "Grid.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "SurfaceRenderer.h"
#include <SFML/Graphics.hpp>

// Configuration variables.
const int GridManager::squaresPerRow = 50;
const int GridManager::squaresPerColumn = 50;
const int GridManager::maxNeighbourRadius = 3;
bool GridManager::debug = true;



//------------------------------------------------------------------------------------

int main(){
    sf::RenderWindow window(sf::VideoMode(1920, 1440), "Sandbox Agent Simulation");
    
    // Set up the surface renderer.
    
    std::cout << "Initialising surface renderer...\n";
    std::ifstream frameSource("/Users/Joel/Desktop/FrameSources.txt");
    std::string framePath;
    while(std::getline(frameSource, framePath)){
        SurfaceRenderer::loadFrame(framePath);
    }
    SurfaceRenderer::setFrame(0);
    std::cout << "Success!\n";
    
    // Set up the grid manager.
    
    std::cout << "Initialising grid manager...\n";
    GridManager::makeZones();
    std::cout << "Success!\n";
    //GridManager::test();
    
    // Set up the agent manager.
    
    std::cout << "Initialising agent manager...\n";
    AgentManager::setTexture(AgentType::ant, "/Users/Joel/GitHub/AntBox/Ant/Ant/ant-graphic.png");
    AgentManager::setWindow(window);
    std::cout << "Success!\n";
    
    // Make an ant.
    
    std::cout << "Making an ant...\n";
    Vector defaultPosition(SurfaceRenderer::getCurrentWidth() / 2, SurfaceRenderer::getCurrentHeight() / 2, SurfaceRenderer::getDepth(SurfaceRenderer::getCurrentWidth() / 2, SurfaceRenderer::getCurrentHeight() / 2));
    //Vector defaultPosition = GridManager::getPoint(2060);
    AgentManager::createAgent(AgentType::ant, defaultPosition);
    std::cout << "Ant made successfully!\n\n";
    
    // Run the main loop.
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        
        // Draw the frame.
        SurfaceRenderer::drawFrame(window);
        
        // Process the agents.
        AgentManager::stepAgents();
        
        window.display();
    }
    return 0;
}

