#pragma once
#include "Bullet.h"

class NotorBangerBullet : public Bullet
{
private:
public:
	NotorBangerBullet();
	~NotorBangerBullet();

	NotorBangerBullet(int _x, int _y, int _dirRight);

	void Fly45();

	void OnCollision(MObject *otherObj, char* sideCollided);
	void Update();
	void Render();
};

