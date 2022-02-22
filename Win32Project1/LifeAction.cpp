#include "stdafx.h"
#include "LifeAction.h"


LifeAction::LifeAction()
{
	action = ActivityType::socialize;
}


LifeAction::LifeAction(ActivityType::ActivityType activity)
{
	action = activity;
}

void LifeAction::ResetWithType(ActivityType::ActivityType activity)
{
	action = activity;
}

