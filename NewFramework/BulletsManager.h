#pragma once
#include <stdio.h>
#include <vector>
#include "Bullet.h"
#include "MegamanBullet.h"

class BulletsManager
{
public:
	//static Bullet* bulletsList;
	static std::vector<Bullet*> bulletsList;

	BulletsManager();
	~BulletsManager();

	static void CreateBullet(Bullet* bullet);
	static void UpdateBullets();
};

