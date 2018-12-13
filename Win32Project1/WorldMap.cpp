#include "stdafx.h"
#include "WorldMap.h"



WorldMap::WorldMap()
{

}


WorldMap::WorldMap(int width, int height)
{
	worldCells.reserve(width);//// , std::vector<std::unique_ptr<WorldCell>>());// , std::vector<std::unique_ptr<WorldCell>>(width));
	worldWidth = width;
	worldHeight = height;
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
	worldWidth = width;
	worldHeight = height;
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
	int currentDistance = 0;
	int currentX = x;
	int currentY = y;
	WorldCell* freeCell = NULL;
	int subcounter = 0;
	int cycleLength = 0;

	while (freeCell == NULL)
	{
		if (currentX < 0 || currentX >= (*this).worldWidth)
		{
			if (x - currentDistance < 0 && x + currentDistance >= (*this).worldWidth
					&& y - currentDistance < 0 && y + currentDistance >= (*this).worldHeight)
			{
				return NULL;
			}
		}
		else if(currentY < 0 || currentY >= (*this).worldHeight)
		{
			if (x - currentDistance < 0 && x + currentDistance >= (*this).worldWidth
					&& y - currentDistance < 0 && y + currentDistance >= (*this).worldHeight)
			{
				return NULL;
			}
		}
		else
		{
			if ((*this).worldCells[currentX][currentY].Occupant == NULL)
			{
				return freeCell = &(*this).worldCells[currentX][currentY];
			}
		}

		if (subcounter >= cycleLength - 1)////2*((2 * (currentDistance - 1)) + 1) + 2*((2 * currentDistance) + 1) - 1)
		{
			currentDistance++;
			cycleLength = WorldMap::GridMapCircumference(currentDistance);
			subcounter = 0;
		}
		else
		{
			subcounter++;
		}


		if (subcounter == 0)
		{
			currentX = x;
			currentY = y + currentDistance;
		}
		else if (subcounter <= currentDistance)
		{
			currentX++;
		}
		else if (subcounter <= (3 * currentDistance))
		{
			currentY--;
		}
		else if (subcounter <= (5 * currentDistance))
		{
			currentX--;
		}
		else if (subcounter <= (7 * currentDistance))
		{
			currentY++;
		}
		else
		{
			currentX++;
		}

	}

	return freeCell;
}



