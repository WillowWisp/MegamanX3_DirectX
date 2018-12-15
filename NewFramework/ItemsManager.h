#pragma once
#include <stdio.h>
#include <vector>
#include "Hp.h"

class ItemsManager
{
public:

	static std::vector<HP*> HPItemsList;

	ItemsManager();
	~ItemsManager();

	static void DropItem(HP* hpItem);
	static void UpdateItems();
};

