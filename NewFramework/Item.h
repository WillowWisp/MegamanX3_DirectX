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
	virtual ~Item();

	void Drop();
	virtual void Update() = 0;
	virtual void Render() = 0;
};

