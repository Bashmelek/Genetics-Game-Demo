#include "stdafx.h"
#include "DivinePlayer.h"


DivinePlayer::DivinePlayer()
{
	currentEnergy = 30;
}


int DivinePlayer::MaxEnergy()
{
	return 150;
}

int DivinePlayer::CurrentEnergy()
{
	return currentEnergy;
}

void DivinePlayer::AddToCurrentEnergy(int amount)
{
	currentEnergy += amount;
	if (currentEnergy > MaxEnergy())
	{
		currentEnergy = MaxEnergy();
	}
}

void DivinePlayer::SetCurrentEnergy(int newamount)
{
	currentEnergy = newamount;
	if (currentEnergy > MaxEnergy())
	{
		currentEnergy = MaxEnergy();
	}
}
