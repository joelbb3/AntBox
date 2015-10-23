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
	if(normal.y == 0)
	{
		Vector tangent = Vector(0,1,0)
	}
	else
	{
		float a = -(normal.x/normal.y);
		Vector tangent = Vector(1,a,0);
	}
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
    std::cout << "velocity: "; newVelocity.print(); std::cout << "\n";
    std::cout << "acceleration: "; newAcceleration.print(); std::cout << "\n";
	// change the velocity by the acceleration * delta of time
    int elapsedTime = stepClock.restart().asSeconds();
    std::cout << "elapsed: " << elapsedTime << "\n";
	newVelocity = newVelocity + (newAcceleration * elapsedTime);
    std::cout << "velocity: "; newVelocity.print(); std::cout << "\n";
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
    sprite.setPosition(position.x, position.y);
    flags.insert(AgentFlags::moved);
    //std::cout << "Step finished!\n";
}


Vector Ant::getClimbMountainForce(){

    Vector normal = GridManager::interpolateNormal(AgentManager::getFace(agentID), position);
	if(normal.x==0 && normal.y==0) // tangent plane is a horizontal plane
	{
		return Vector(0,0,0);
	}
	if(normal.y == 0)
	{
		Vector tangent = Vector(0,1,0)
	}
	else
	{
		float a = -(normal.x/normal.y);
		Vector tangent = Vector(1,a,0);
	}
	Vector steepest = normal.cross(tangent) / normal.cross(tangent).magnitude();
	// z component must be between -1 and 1
	if(steepest.z < 0)
		steepest = steepest * -1;
	// z component between 0 and 1, the closer to 1, the harder it should be to get up
	steepest = steepest * 5* pow(steepest.z -1,2);
	return steepest;
}





