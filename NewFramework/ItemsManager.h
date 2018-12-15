#pragma once
#include <stdio.h>
#include <vector>
#include "HP.h"
#include "GameLog.h"

class ItemsManager
{
public:

	static std::vector<HP*> HPItemsList;

	ItemsManager();
	~ItemsManager();

	static void DropItem(HP* hpItem);
	static void DropHPItem(int x, int y);
	static void UpdateItems();
	static void RenderItems();
};

