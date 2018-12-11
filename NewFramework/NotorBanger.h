#pragma once
#include "MObject.h"

#define ANIM_DELAY 2

#define STATE_IDLE 0
#define STATE_PREPARE 1
#define STATE_JUMP 2
#define STATE_45_UP 3
#define STATE_90_UP 4
#define STATE_45_DOWN 5
#define STATE_90_DOWN 6
#define STATE_45_SHOOT 7
#define STATE_90_SHOOT 8

class NotorBanger : public MObject
{
	MObject* player;

	void SetState(int newState);
public:
	NotorBanger(MObject* _player);
	~NotorBanger();

	void Update();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
};

