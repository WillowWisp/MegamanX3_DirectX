#pragma once
#include "Item.h"

#define HEAL_AMOUNT_BIG 5
#define HEAL_AMOUNT_SMALL 2
#define HP_ANIM_DELAY 1
#define HP_STATE_DROPPING 0
#define HP_STATE_IDLE 1
#define HP_STATE_DISAPPEARING 2
#define HP_DISAPPEARING_ANIM_TIME 100
#define HP_IDLE_ANIM_TIME 400

class HP :
	public Item
{
public:
	int heal;
	int level; //big == 2, small == 1, random == 0
	//bool isUsed;

	HP();
	HP(int x, int y, int level = 0);
	~HP();

	void SetState(int newState);
	void Update();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
	void Disappear(); //Forced disappearing
};

