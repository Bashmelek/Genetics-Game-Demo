#pragma once



#ifndef LIFEACTIONDEF
#define LIFEACTIONDEF

#include <vector>

namespace ActivityType
{
	enum ActivityType { eat, sleep, copulate, socialize, gatherFood };
}

class LifeAction
{
public:
	LifeAction();
	LifeAction(ActivityType::ActivityType activity);
	ActivityType::ActivityType action;

	void ResetWithType(ActivityType::ActivityType activity);
};




#endif LIFEACTIONDEF

