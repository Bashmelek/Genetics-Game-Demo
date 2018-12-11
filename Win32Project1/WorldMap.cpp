#include "stdafx.h"
#include "WorldMap.h"



WorldMap::WorldMap()
{

}


WorldMap::WorldMap(int width, int height)
{
	worldCells.reserve(width);//// , std::vector<std::unique_ptr<WorldCell>>());// , std::vector<std::unique_ptr<WorldCell>>(width));
	for (int w = 0; w < width; w++)
	{
		worldCells.push_back(std::vector<WorldCell>());
		worldCells[w].reserve(height);
		for (int h = 0; h < height; h++)
		{
			worldCells[w].push_back(WorldCell(w,h));
		}
	}
}

void WorldMap::Initialize(int width, int height)
{
	worldCells.reserve(width);//// , std::vector<std::unique_ptr<WorldCell>>());// , std::vector<std::unique_ptr<WorldCell>>(width));
	for (int w = 0; w < width; w++)
	{
		worldCells.push_back(std::vector<WorldCell>());
		worldCells[w].reserve(height);
		for (int h = 0; h < height; h++)
		{
			worldCells[w].push_back(WorldCell(w, h));
		}
	}
}

WorldCell* WorldMap::FindNearestOpenCell(int x, int y)
{
	return &worldCells[x][y];
}

