#pragma once
#include "Enemy.h"
#include "Megaman.h"
#include "UI.h"
#include "GameLog.h"
//Them chu boss vi Byte bi trung ten voi class co san

#define BYTE_ANIM_DELAY 1
#define BYTE_STATE_APPEARING 0
#define BYTE_STATE_IDLE 1
#define BYTE_STATE_THROWING_MINE 2
#define BYTE_STATE_PREPARING 3
#define BYTE_STATE_CHARGING_AT_PLAYER 4
#define BYTE_STATE_PUNCHING_TO_CEIL 5
#define BYTE_STATE_PUNCHING_TO_WALL 6
#define BYTE_DESCENDING_SPEED 5
#define BYTE_CHARGING_SPEED 5
#define BYTE_STATE_TIME_IDLE 30
#define BYTE_STATE_TIME_PREPARING 30


class ByteBoss :
	public Enemy
{

	Megaman* player;
	int groundY;
	int renderX;
	int renderY;
public:
	ByteBoss();
	~ByteBoss();
	ByteBoss(Megaman* _player, int _x, int _y);

	void SetState(int newState);
	void SetRenderXY();
	void AdjustPosition();
	void Update();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
};

