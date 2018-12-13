#pragma once

////#include "GameViewState.h"
#include <time.h> 
#include <string>
#include "WorldMap.h"

#ifndef GAMEOBJDEF
#define GAMEOBJDEF


class GameObject
{
public:
	GameObject();
	GameObject(bool isDummy);
	////virtual void DrawObject();
//protected:
	bool isVisibleInWorld;
	int worldX;
	int worldY;
	int currentFrame;
	int totalFrames;

	void SetWorldPositionDirect(int x, int y);
	void SetWorldPositionToClosestPoint(int x, int y);
};




#endif GAMEOBJDEF
