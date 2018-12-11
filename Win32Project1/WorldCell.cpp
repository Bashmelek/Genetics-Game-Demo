#include "stdafx.h"
#include "WorldCell.h"



WorldCell::WorldCell()
{

}


WorldCell::WorldCell(int x, int y)
{
	(*this).x = x;
	(*this).y = y;
	Occupant = NULL;
}
