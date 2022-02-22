#include "stdafx.h"
#include "Inventory.h"





int Inventory::NumberOfItemTypes = 2;

Inventory::Inventory()
{
	//NumberOfItemTypes = 2;
	NumberOfItemTypes = 2;

	InventoryItems.reserve(NumberOfItemTypes);
}
