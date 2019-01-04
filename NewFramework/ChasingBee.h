#pragma once

#include "Bullet.h"
#include "Megaman.h"

#define ANIM_DELAY 0
#define FLY_SPEED 2

class ChasingBee : public Bullet
{
	Megaman* player;

	int slowUpdate;

	void ChasePlayer();
public:
	ChasingBee(Megaman* _player, D3DXVECTOR2 _firePoint);
	~ChasingBee();

	void OnCollision(MObject *otherObj, char* sideCollided);
	void Update();
	void Render();
};

