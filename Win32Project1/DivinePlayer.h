#pragma once


#ifndef DIVINEPLAYERDEF
#define DIVINEPLAYERDEF


class DivinePlayer
{
public:
	DivinePlayer();
	int CurrentEnergy();
	int MaxEnergy();
	void DivinePlayer::AddToCurrentEnergy(int amount);
	void DivinePlayer::SetCurrentEnergy(int newamount);

private:
	int currentEnergy;
};






#endif DIVINEPLAYERDEF
