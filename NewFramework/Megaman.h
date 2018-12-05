#pragma once
#include "MObject.h"
#include "Input.h"

#define STATE_IDLE 0
#define STATE_RUNNING 1
#define STATE_SHOOTING 2
#define STATE_RUNNING_N_SHOOTING 3
#define STATE_JUMPING 4
#define STATE_FALLING 5
#define STATE_DASHING 6
#define STATE_DASHING_N_SHOOTING 7
#define ANIM_DELAY 1
#define MEGAMAN_SPEED 5
#define DASH_SPEED 10
#define DASH_DURATION 10
#define JUMP_SPEED 17
#define GRAVITY -0.25
#define GROUND_Y 380

class Megaman :
	public MObject
{
public:
	Megaman();
	~Megaman();

	//

	//
	void SetState(int);
	void Update();
};

