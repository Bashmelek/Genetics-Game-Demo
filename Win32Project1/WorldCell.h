#pragma once



#ifndef GAMECELLDEF
#define GAMECELLDEF

class GameObject;

class WorldCell
{
public:
	WorldCell();
	WorldCell(int x, int y);

	int x;
	int y;

	GameObject* Occupant;
};




#endif GAMECELLDEF

