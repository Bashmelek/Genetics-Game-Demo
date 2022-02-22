#include "stdafx.h"
#include "Strategy.h"



Strategy::Strategy()
{
	//plannedActions.reserve(ACTIONPOINTS);
	for (int i = 0; i < ACTIONPOINTS; i++)
	{
		plannedActions.push_back(LifeAction(ActivityType::sleep));
	}
}
