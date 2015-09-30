#pragma once
#include "stdafx.h"
#include <vector>


class AgentState;

enum AffordanceType {
	damage
};

class Affordance {
protected:
	AgentState* holderState;
	virtual void operator()(std::vector<void*> args = std::vector<void*>());
	Affordance(AgentState* holderState) : holderState(holderState) {};
public:
	AffordanceType type;
};

class AffordanceFactory {
private:
	class Damage : public Affordance {
		int dmgResist;
		void operator()(std::vector<void*> args = std::vector<void*>());
	public:
		Damage(AgentState* holderState) : Affordance(holderState) {};
	};

public:
	Affordance make(AgentState* holderState, AffordanceType type) {
		switch (type) {
		case damage:
			return Damage(holderState);
		}
	}

};