#pragma once


#include "WorldCell.h"
#include <memory>
#include <vector>

#ifndef GAMEWORLDDEF
#define GAMEWORLDDEF

#define DEFAULTWORLDWIDTH 10
#define DEFAULTWORLDHEIGHT 10


class WorldMap
{
public:
	WorldMap();
	WorldMap(int width, int height);
	void Initialize(int width, int height);
	//std::vector<std::vector<std::unique_ptr<WorldCell>>> worldCells;
	std::vector<std::vector<WorldCell>> worldCells;

	static WorldMap& TheWorld()
	{
		static WorldMap theMap;

		return theMap;
	}

	WorldCell* FindNearestOpenCell(int x, int y);
};




#endif GAMEWORLDDEF
