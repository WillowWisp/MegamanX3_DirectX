#include "Bullet.h"



Bullet::Bullet()
{
	tag = (char*)"bullet";
}


Bullet::~Bullet()
{
}

Bullet::Bullet(int _x, int _y) {
	x = _x;
	y = _y;
}