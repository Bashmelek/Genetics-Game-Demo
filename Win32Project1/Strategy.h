#pragma once



#ifndef STRATEGYDEF
#define STRATEGYDEF

#define ACTIONPOINTS 96

#include "LifeAction.h"

class Strategy
{
public:
	Strategy();
	std::vector<LifeAction> plannedActions;

};




#endif STRATEGYDEF

