#pragma once

#include "Bullet.h"

class HeadGunnerMissile : public Bullet
{
	MObject* target;
public:
	HeadGunnerMissile(MObject* _target, D3DXVECTOR2 _firePoint, int _dirRight);
	~HeadGunnerMissile();

	void OnCollision(MObject *otherObj, char* sideCollided);
	void Update();
	void Render();
};

