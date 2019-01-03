#pragma once

#include "Bullet.h"

#define ANIM_DELAY 0
#define FLY_SPEED 6

class MiniBee : public Bullet
{
public:
	MiniBee(D3DXVECTOR2 des, D3DXVECTOR2 _firePoint, int _dirRight);
	~MiniBee();

	void OnCollision(MObject *otherObj, char* sideCollided);
	void Update();
	void Render();
};

