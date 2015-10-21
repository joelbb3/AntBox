#pragma once
#include <memory>
#include <vector>


struct AgentState;

enum AffordanceType {
	damage,
    strengthen_pheremone
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

    class StrengthenPheremone : public Affordance{
        void operator()(std::vector<void*> args = std::vector<void*>());
    };
    
public:
	std::unique_ptr<Affordance> make(AgentState* holderState, AffordanceType type) {
		switch (type) {
		case damage:
			return std::unique_ptr<Affordance>(new Damage(holderState));
		}
	}

};