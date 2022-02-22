#pragma once



#ifndef INVENTORYDEF
#define INVENTORYDEF


#include <vector>
#include <list>

class InventoryItem
{
public:
	//InventoryItem();
};


class Inventory
{
public:
	Inventory();
	std::vector<std::list<InventoryItem>> InventoryItems;

	static int NumberOfItemTypes;
};




#endif INVENTORYDEF

