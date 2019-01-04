#pragma once
#include "Bullet.h"

class HelitBullet : public Bullet
{
public:
	HelitBullet();
	HelitBullet(D3DXVECTOR2 _firePoint, int _dirRight);
	~HelitBullet();

	void OnCollision(MObject *otherObj, char* sideCollided);
	void Update();
	void Render();
};

