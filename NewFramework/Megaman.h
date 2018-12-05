#pragma once
#include "MObject.h"
#include "Input.h"

#define STATE_IDLE 0
#define STATE_RUNNING 1
#define STATE_SHOOTING 2
#define STATE_RUNNING_N_SHOOTING 3
#define STATE_JUMPING 4
#define ANIM_DELAY 1
#define MEGAMAN_SPEED 5

class Megaman :
	public MObject
{
public:
	Megaman();
	~Megaman();

	//
	void SetState(int);
	void Update();
};

