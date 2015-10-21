#ifndef ANT_ANT_H_
#define ANT_ANT_H_

#include "Geometry.h"
#include "Agent.h"

enum ForceType{
	climbMountianForce
};

class Ant : public Agent{

private:
	Vector _velocity;
	double _maxForce;
	double _maxSpeed;
	double _speed;
	double _mass;
	ForceType _forceType;
	Vector constrainToSurface(const Vector velocity);
	Vector getSteepestDown();

	Vector getSteeringForce(){
		switch(_forceType){
		case climbMountianForce:
			return getClimbMountainForce();
			break;
		default:
			return getClimbMountainForce();
		}
	}

public:
    Ant(std::string& texturePath, Vector startingPosition): _velocity(Vector(0,0,0)), _maxForce(10),_maxSpeed(10), Agent(texturePath, startingPosition){
    }
	~Ant(){};
	void step();
	Vector getClimbMountainForce();

	double getMaxForce() const {
		return _maxForce;
	}

	void setMaxForce(double maxForce) {
		_maxForce = maxForce;
	}

	double getMaxSpeed() const {
		return _maxSpeed;
	}

	void setMaxSpeed(double maxSpeed) {
		_maxSpeed = maxSpeed;
	}

	const Vector& getPosition() const {
		return position;
	}

	void setPosition(const Vector& position) {
		this->position = position;
	}

	const Vector& getVelocity() const {
		return _velocity;
	}

	double getSpeed() const {
		return _speed;
	}

	void setSpeed(double speed) {
		_speed = speed;
	}

	void setVelocity(const Vector& velocity) {
		this->_velocity = velocity;
	}

	ForceType getForceType() const {
		return _forceType;
	}

	void setForceType(ForceType forceType) {
		_forceType = forceType;
	}

	double getMass() const {
		return _mass;
	}

	void setMass(double mass) {
		_mass = mass;
	}
};



#endif /* ANT_ANT_H_ */
