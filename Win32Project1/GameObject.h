#pragma once

////#include "GameViewState.h"
#include <time.h> 
#include <string>

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
};




#endif GAMEOBJDEF
