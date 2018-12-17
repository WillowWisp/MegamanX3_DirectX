#pragma once
#include <stdio.h>
#include <vector>
#include "NotorBanger.h"
#include "HeadGunner.h"
#include "Helit.h"
#include "Random.h"
#include "Effects.h"

#define TYPE_RANDOM -1
#define TYPE_NOTOR_BANGER 0
#define TYPE_HEAD_GUNNER 1
#define TYPE_HELIT 2

class EnemiesManager
{
public:
	static std::vector<Enemy*> enemiesList;

	EnemiesManager();
	~EnemiesManager();

	static void SpawnEnemy(Enemy* enemy);
	static void SpawnEnemy(MObject* player, int x, int y, int type = TYPE_RANDOM, int dirRight = 1);
	static void UpdateEnemies();
	static void RenderEnemies();
};

