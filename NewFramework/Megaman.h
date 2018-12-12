#pragma once
#include <stdlib.h>
#include "MObject.h"
#include "Input.h"
#include "GameLog.h"

#define STATE_IDLE 0
#define STATE_RUNNING 1
#define STATE_SHOOTING 2
#define STATE_RUNNING_N_SHOOTING 3
#define STATE_JUMPING 4
#define STATE_FALLING 5
#define STATE_DASHING 6
//#define STATE_DASHING_N_SHOOTING 7
//#define STATE_JUMPING_N_SHOOTING 8
#define STATE_WALL_SLIDING 9
#define STATE_WALL_KICKING 10
#define ANIM_DELAY 1
#define MEGAMAN_SPEED 5
#define DASH_SPEED 13
#define DASH_DURATION 15
#define JUMP_SPEED 17
#define WALL_JUMP_SPEED 20
#define WALL_DASH_JUMP_SPEED 5
#define WALL_DASH_BOUNCE_SPEED 10
#define GRAVITY -0.25 
#define KICK_ANTI_FORCE 5
#define WALL_SLIDE_SPEED -7
#define GROUND_Y 390
#define LEFTWALL_X 0
#define RIGHTWALL_X 227

class Megaman :
	public MObject
{
	MObject *collideObject;
public:
	Megaman();
	~Megaman();

	//variables to manage megaman's shitton amount of states
	bool isHitGround;
	bool isHitWallLeft;
	bool isHitWallRight;
	int curGroundY;
	int curCeilY;
	int curLeftWallX;
	int curRightWallX;
	//bool isHitWall;

	bool jumpHold;
	bool dashHold;
	bool shootHold;
	bool leftHold;
	bool rightHold;
	bool moving;
	bool jumpAfterDash;
	bool inMidAir;
	bool wallJump;
	bool dashKick;

	//
	//void SetWidthHeight();
	void SetState(int);
	void Update();
	bool HitGround();
	bool HitCeil();
	bool HitWall();
	bool CloseToWall();

	void OnCollision(MObject *otherObj, char* sideCollided);

	Camera *camera;
	void SetCamera(Camera *newCamera);
};

