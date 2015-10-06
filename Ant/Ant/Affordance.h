#pragma once
#include <memory>
#include <vector>


struct AgentState;

enum AffordanceType {
	damage
};

class Affordance {
protected:
	AgentState* holderState;
	virtual void operator()(std::vector<void*> args = std::vector<void*>()) = 0;
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
	std::unique_ptr<Affordance> make(AgentState* holderState, AffordanceType type) {
		switch (type) {
		case damage:
			return std::unique_ptr<Affordance>(new Damage(holderState));
		}
	}

};