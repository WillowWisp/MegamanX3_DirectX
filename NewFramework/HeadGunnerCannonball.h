#pragma once

#include "Bullet.h"

class HeadGunnerCannonball : public Bullet
{
	MObject* target;
public:
	HeadGunnerCannonball(MObject* _target, D3DXVECTOR2 _firePoint, int _dirRight);
	~HeadGunnerCannonball();

	void OnCollision(MObject *otherObj, char* sideCollided);
	void Update();
	void Render();
};

