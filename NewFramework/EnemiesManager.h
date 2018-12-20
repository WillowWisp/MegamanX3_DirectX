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
#define NOTOR_BANGER_HEIGHT 96
#define HEAD_GUNNER_HEIGHT 90
#define HELIT_HEIGHT 58

class EnemiesManager
{
public:
	static std::vector<Enemy*> enemiesList;
	static std::vector<RECT> enemySpawnSpots;
	static std::vector<int> enemyTypeAtSpot;
	static std::vector<int> enemyDirAtSpot;
	static std::vector<bool> enemyIsSpawnedAtSpot;

	EnemiesManager();
	~EnemiesManager();
	
	static void SpawnEnemy(Enemy* enemy);
	static void SpawnEnemy(MObject* player, int x, int y, int type = TYPE_RANDOM, int dirRight = 1);
	static void SpawnEnemiesNearCamera(MObject* player);
	static void UpdateEnemies();
	static void RenderEnemies();
};

