#pragma once


#include "WorldCell.h"
#include <memory>
#include <vector>

#ifndef GAMEWORLDDEF
#define GAMEWORLDDEF

#define DEFAULTWORLDWIDTH 15
#define DEFAULTWORLDHEIGHT 15


class WorldMap
{
public:
	WorldMap();
	WorldMap(int width, int height);
	int worldWidth;
	int worldHeight;
	void Initialize(int width, int height);
	//std::vector<std::vector<std::unique_ptr<WorldCell>>> worldCells;
	std::vector<std::vector<WorldCell>> worldCells;

	static WorldMap& TheWorld()
	{
		static WorldMap theMap;

		return theMap;
	}

	static int GridMapCircumference(int dist)
	{
		return 4 * (((2 * (dist - 1)) + 1)) + 4;
	}

	WorldCell* FindNearestOpenCell(int x, int y);
};




#endif GAMEWORLDDEF
