#include "Ant.h"
#include "Grid.h"
#include "AgentManager.h"
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

Vector Ant::constrainToSurface (const Vector velocity)

{
	if(velocity.x==0&& velocity.y==0)
	{
		Vector down = getSteepestDown();
		float temp = sqrt(pow(down.x,2) + pow(down.y,2));
		down = down * velocity.z*sin(atan(down.z/temp));
		return down;
	}

	//else, take the change in x and y
	int x = position.x + velocity.x;
	int y = position.y + velocity.y;
	//find the corresponding zs
    int z = GridManager::interpolateHeight(AgentManager::getFace(agentID), position);
	//find the vector in the direction of movement constained to surface
	Vector direction = Vector(x - position.x, y - position.y, z - position.z);
    direction = direction / direction.magnitude();
    int speed = velocity.dot(direction);
	Vector newVelocity = speed * direction;
	return newVelocity;
}

Vector Ant::getSteepestDown()
{
    Vector normal = GridManager::interpolateNormal(AgentManager::getFace(agentID), position);
	if(normal.x == 0 && normal.x == 0) // tangent plane is a horizontal plane
	{
		return Vector(0,0,0);
	}

	float a = -(normal.x/normal.y);
	Vector tangent = Vector(1,a,0);
    Vector steepest = normal.cross(tangent) / normal.cross(tangent).magnitude();
	// z component must be between -1 and 1
	if(steepest.z > 0)
		steepest = steepest * -1;
	return steepest;
}


void Ant::step(){
    std::cout << "Ant #" << agentID << " making internal step.\n";
	Vector force = getSteeringForce();
	// f / m = a
	Vector newAcceleration = (force / _mass);
	// get velocity
	Vector newVelocity = _velocity;

	// change the velocity by the acceleration * delta of time
    int elapsedTime = stepClock.getElapsedTime().asMilliseconds();
	newVelocity = newVelocity + (newAcceleration * elapsedTime);
	// set velocity and speed
	setSpeed(newVelocity.magnitude());
	if(_speed > _maxSpeed)
	{
        newVelocity = newVelocity / newVelocity.magnitude();
		newVelocity = newVelocity * _maxSpeed;
		setSpeed(_maxSpeed);
	}


	constrainToSurface(newVelocity);
	setVelocity(newVelocity);
	setPosition (position + (newVelocity * elapsedTime));
	
	// check sprite is still within boundary
	if(position.x < 0)
		position.x = 0;
	if(position.y < 0)
		position.y = 0;
	int max_x = GridManager::sandboxWidth;
	int max_y = GridManager::sandboxHeight;
	if(position.x > max_x)
		position.x = max_x;
	if(position.y > max_y)
		position.y = max_y;
	
	
    sprite.setPosition(position.x, position.y);
    flags.insert(AgentFlags::moved);
    //std::cout << "Step finished!\n";
}


Vector Ant::getClimbMountainForce(){

    Vector normal = GridManager::interpolateNormal(AgentManager::getFace(agentID), position);
	if(normal.x==0 && normal.x==0) // tangent plane is a horizontal plane
	{
		return Vector(0,0,0);
	}

	float a = -(normal.x/normal.y);
	Vector tangent = Vector(1,a,0);
    Vector steepest = normal.cross(tangent) / normal.cross(tangent).magnitude();
	// z component must be between -1 and 1
	if(steepest.z < 0)
		steepest = steepest * -1;
	// z component between 0 and 1, the closer to 1, the harder it should be to get up
	steepest = steepest * 5* pow(steepest.z -1,2);
	return steepest;
}


Vector Ant::getCentralTendencyForce(){
	
	int max_x = GridManager::sandboxWidth;
	int max_y = GridManager::sandboxHeight;
	
	Vector center = (max_x/2, max_y/2, 0);
	// I don't know how to find the corresponding z coordinate!
	// so I left it as zero. :(
	
	float distance = sqrt(pow(position.x-center.x,2) + pow(position.y-center.y,2) + pow(position.z-center.z,2));
	Vector force = center - position;
	
	force = force / force.magnitude();
	// setting the magnitude to be inverse squared proportional to the distance from the edge
	// (so will be pushed further in when closer to the edge)
	// assumption: max_y is less than max_x
	force = force * (1/(pow(distance - max_y/2, 2)));
	return force;
}


