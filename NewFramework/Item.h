#pragma once
#include "MObject.h"
#include "Random.h"

#define ITEM_DROP_SPEED 20

class Item :
	public MObject
{
public:
	bool isDisappeared;
	int state_t;


	Item();
	~Item();

	void Drop();
};

