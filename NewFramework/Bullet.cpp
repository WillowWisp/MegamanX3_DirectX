#include "Bullet.h"



Bullet::Bullet()
{
	tag = (char*)"bullet";
	isDestroyed = false;
	state_t = -1;
}


Bullet::~Bullet()
{
}

Bullet::Bullet(int _x, int _y) {
	x = _x;
	y = _y;
}

void Bullet::Render() {

}

void Bullet::Destroyed() {
	isDestroyed = true;
}