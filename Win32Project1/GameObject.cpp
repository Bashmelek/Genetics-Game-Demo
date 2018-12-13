#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::GameObject(bool isDummy)
{

}


void GameObject::SetWorldPositionDirect(int x, int y)
{
	(*this).worldX = x;
	(*this).worldY = y;

	WorldMap::TheWorld().worldCells[x][y].Occupant = this;
}


void GameObject::SetWorldPositionToClosestPoint(int x, int y)
{
	if (WorldMap::TheWorld().worldCells[this->worldX][this->worldY].Occupant == this)
	{
		WorldMap::TheWorld().worldCells[this->worldX][this->worldY].Occupant = NULL;
	}

	WorldCell * theCell = WorldMap::TheWorld().FindNearestOpenCell(x, y);

	(*this).worldX = theCell->x;
	(*this).worldY = theCell->y;

	(*theCell).Occupant = this;
}

