#pragma once

#include "MObject.h"
#include "HeadGunnerMissile.h"
#include "HeadGunnerCannonball.h"
#include <vector>
#include "Enemy.h"
#include "BulletsManager.h"

#define ANIM_DELAY 2

#define STATE_IDLE 0
#define STATE_PREPARE_SHOOT_1 1
#define STATE_SHOOT_1 2
#define STATE_PREPARE_SHOOT_2 3
#define STATE_SHOOT_2 4
#define STATE_SHOOT_CANON 5

class HeadGunner : public Enemy
{
	MObject* player;
	D3DXVECTOR2 firePoint;

	//std::vector<Bullet*> bulletList;

	void SetState(int newState);

	void ShootMissile();
	void ShootCannonball();
public:
	HeadGunner(MObject* _player, int _x, int _y, int _dirRight = 1);
	~HeadGunner();

	void Update();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
};

