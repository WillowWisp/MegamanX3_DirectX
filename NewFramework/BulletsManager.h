#pragma once
#include <stdio.h>
#include <vector>
#include "Bullet.h"
#include "MegamanBullet.h"
#include "Effects.h"

class BulletsManager
{
public:
	//static Bullet* MegamanBulletsList;
	static std::vector<MegamanBullet*> MegamanBulletsList;
	static std::vector<Bullet*> EnemyBulletsList;

	BulletsManager();
	~BulletsManager();

	static void CreateBullet(Bullet* bullet);	//For enemy bullets
	static void CreateBullet(MegamanBullet* megamanBullet); //For Megaman bullets
	static void UpdateBullets();
	static void RenderBullets();
};

