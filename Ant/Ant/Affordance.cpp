#include "stdafx.h"
#include "Affordance.h"
#include "Agent.h"

void AffordanceFactory::Damage::operator()(std::vector<void*> args) {
	int dmgIn = *static_cast<int*>(args[0]);
}
