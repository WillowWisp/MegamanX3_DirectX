#include "EnemiesManager.h"

std::vector<Enemy*> EnemiesManager::enemiesList;

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

void EnemiesManager::UpdateEnemies() {
	for (int i = 0; i < enemiesList.size(); i++) {
		if (enemiesList[i]->isDestroyed) {
			Effects::CreateExplosion(enemiesList[i]->x, enemiesList[i]->y);
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
