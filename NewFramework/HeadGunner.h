#pragma once

#include "MObject.h"
#include "HeadGunnerMissile.h"
#include <vector>

#define ANIM_DELAY 2

#define STATE_IDLE 0
#define STATE_PREPARE_SHOOT_1 1
#define STATE_SHOOT_1 2
#define STATE_PREPARE_SHOOT_2 3
#define STATE_SHOOT_2 4
#define STATE_SHOOT_CANON 5

class HeadGunner : public MObject
{
	MObject* player;
	D3DXVECTOR2 firePoint;

	std::vector<HeadGunnerMissile*> missileList;

	void SetState(int newState);

	void ShootMissile();
public:
	HeadGunner(MObject* _player, int _x, int _y, int _dirRight);
	~HeadGunner();

	void Update();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
};

