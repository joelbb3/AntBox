#include <vector>
#include "Affordance.h"
#include "Agent.h"

void AffordanceFactory::StrengthenPheremone::operator()(std::vector<void*> args){
    int changeAmount = *(static_cast<int*>(args[0]));
    holderState->pherStrength += changeAmount;
}