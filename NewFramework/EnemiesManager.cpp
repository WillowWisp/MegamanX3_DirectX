#include "EnemiesManager.h"


std::vector<Enemy*> EnemiesManager::enemiesList;
std::vector<RECT> EnemiesManager::enemySpawnSpots;
std::vector<int> EnemiesManager::enemyTypeAtSpot;
std::vector<int> EnemiesManager::enemyDirAtSpot;
std::vector<bool> EnemiesManager::enemyIsSpawnedAtSpot;

EnemiesManager::EnemiesManager()
{
}


EnemiesManager::~EnemiesManager()
{
	for (int i = 0; i < enemiesList.size(); i++) {
		delete enemiesList[i];
	}
}

void EnemiesManager::SpawnEnemy(Enemy* enemy) {
	enemiesList.push_back(enemy);
}

void EnemiesManager::SpawnEnemy(MObject* player, int x, int y, int type, int dirRight) {
	if (type == TYPE_RANDOM) {
	//Spawn random enemy
		type = Random::RandInt(0, 3);
	}

	switch (type) {
	case TYPE_NOTOR_BANGER:
		enemiesList.push_back(new NotorBanger(player, x, y));
		break;
	case TYPE_HEAD_GUNNER:
		enemiesList.push_back(new HeadGunner(player, x, y, dirRight));
		break;
	case TYPE_HELIT:
		enemiesList.push_back(new Helit(player, x, y, dirRight));
		break;
	default:
		break;
	}
}

void EnemiesManager::SpawnEnemiesNearCamera(MObject* player) {
	RECT cameraRect = GameGlobal::camera->GetBound();
	//cameraRect.left -= 100;
	//cameraRect.right += 100;
	//cameraRect.top -= 100;
	//cameraRect.bottom += 100;
	RECT outerCameraRect; //a rect surrounding cameraRect, enemies spawn in this region
	outerCameraRect.left = cameraRect.left - OUTER_CAMERA_ZONE;
	outerCameraRect.right = cameraRect.right + OUTER_CAMERA_ZONE;
	outerCameraRect.top = cameraRect.top - OUTER_CAMERA_ZONE;
	outerCameraRect.bottom = cameraRect.bottom + OUTER_CAMERA_ZONE;

	for (int i = 0; i < enemySpawnSpots.size(); i++) {
		if (!GameGlobal::IsContain(cameraRect, enemySpawnSpots[i])
			&& !GameGlobal::IsIntersect(cameraRect, enemySpawnSpots[i])
			&& GameGlobal::IsContain(outerCameraRect, enemySpawnSpots[i])	//enemy in  outer camera region
			&& !enemyIsSpawnedAtSpot[i])									//enemy hasn't been spawned
		{
			//spawn
			enemyIsSpawnedAtSpot[i] = true;

			if (enemyTypeAtSpot[i] == TYPE_NOTOR_BANGER) {
				int x = enemySpawnSpots[i].left + (enemySpawnSpots[i].right - enemySpawnSpots[i].left) / 2;
				int y = enemySpawnSpots[i].bottom - NOTOR_BANGER_HEIGHT / 2;
				SpawnEnemy(player, x, y, TYPE_NOTOR_BANGER);
			}
			else if (enemyTypeAtSpot[i] == TYPE_HEAD_GUNNER) {
				int x = enemySpawnSpots[i].left + (enemySpawnSpots[i].right - enemySpawnSpots[i].left) / 2;
				int y = enemySpawnSpots[i].bottom - HEAD_GUNNER_HEIGHT / 2;
				SpawnEnemy(player, x, y, TYPE_HEAD_GUNNER, enemyDirAtSpot[i]);
			}
			else if (enemyTypeAtSpot[i] == TYPE_HELIT) {
				int x = enemySpawnSpots[i].left + (enemySpawnSpots[i].right - enemySpawnSpots[i].left) / 2;
				int y = enemySpawnSpots[i].bottom - HELIT_HEIGHT / 2;
				SpawnEnemy(player, x, y, TYPE_HELIT, enemyDirAtSpot[i]);
			}

			enemiesList[enemiesList.size() - 1]->id = i;
		}
		//else if ((GameGlobal::IsContain(cameraRect, enemySpawnSpots[i])			//enemy in camera region
		//		|| !GameGlobal::IsContain(outerCameraRect, enemySpawnSpots[i]))	//enemy out of outer region
		//		&& enemyIsSpawnedAtSpot[i])
		//{
		//	//de-spawn
		//	enemyIsSpawnedAtSpot[i] = false;
		//}
	}
}

void EnemiesManager::UpdateEnemies() {
	RECT cameraRect = GameGlobal::camera->GetBound();
	RECT outerCameraRect; //a rect surrounding cameraRect, enemies spawn in this region
	outerCameraRect.left = cameraRect.left - OUTER_CAMERA_ZONE;
	outerCameraRect.right = cameraRect.right + OUTER_CAMERA_ZONE;
	outerCameraRect.top = cameraRect.top - OUTER_CAMERA_ZONE;
	outerCameraRect.bottom = cameraRect.bottom + OUTER_CAMERA_ZONE;

	for (int i = 0; i < enemiesList.size(); i++) {
		if (enemiesList[i]->isDestroyed) {
			Effects::CreateExplosion(enemiesList[i]->x, enemiesList[i]->y);
			enemyIsSpawnedAtSpot[enemiesList[i]->id] = false;
			delete enemiesList[i];
			enemiesList.erase(enemiesList.begin() + i);
			i--;

		}
		else if (!GameGlobal::IsContain(outerCameraRect, enemiesList[i]->GetRect())
				&& !GameGlobal::IsIntersect(outerCameraRect, enemiesList[i]->GetRect())) 
		{
			enemyIsSpawnedAtSpot[enemiesList[i]->id] = false;
			delete enemiesList[i];
			enemiesList.erase(enemiesList.begin() + i);
			i--;
		}
		else {
			enemiesList[i]->Update();
		}
	}
}

void EnemiesManager::RenderEnemies() {
	for (int i = 0; i < enemiesList.size(); i++) {
		enemiesList[i]->Render();
	}
}
