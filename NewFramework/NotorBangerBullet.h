#pragma once
#include "Bullet.h"

class NotorBangerBullet : public Bullet
{
private:
public:
	NotorBangerBullet();
	~NotorBangerBullet();

	NotorBangerBullet(D3DXVECTOR2 _firePoint, int _dirRight);

	void Fly45();
	void Fly90();

	void OnCollision(MObject *otherObj, char* sideCollided);
	void Update();
	void Render();
};

