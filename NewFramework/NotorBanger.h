﻿#pragma once
#include "MObject.h"
#include "NotorBangerBullet.h"
#include <vector>
#include <cmath>

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
	MObject* player; //Target

	std::vector<NotorBangerBullet*> bulletList;
	char* angle; //Góc bắn
	D3DXVECTOR2 firePoint; //Điểm nhả đạn

	void SetState(int newState);

	void Shoot45();
	void Shoot90();
public:
	NotorBanger(MObject* _player);
	~NotorBanger();

	void Updates();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
};

