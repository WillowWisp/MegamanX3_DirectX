#include "Enemy.h"



Enemy::Enemy()
{
	isDestroyed = false;
	tag = (char*)"enemy";
}


Enemy::~Enemy()
{
	//
}

void Enemy::TakeDmg(int damage) {
	HP -= damage;
	if (HP <= 0) {
		Destroyed();
		return;
	}
}

void Enemy::Destroyed() {
	isDestroyed = true;
	ItemsManager::DropHPItem(x, y);
}
