#include "BulletsManager.h"

std::vector<MegamanBullet*> BulletsManager::MegamanBulletsList;
std::vector<Bullet*> BulletsManager::EnemyBulletsList;

BulletsManager::BulletsManager()
{
}


BulletsManager::~BulletsManager()
{
	for (int i = 0; i < MegamanBulletsList.size(); i++) {
		delete MegamanBulletsList.at(i);
	}
	for (int i = 0; i < EnemyBulletsList.size(); i++) {
		delete EnemyBulletsList.at(i);
	}
}

void BulletsManager::CreateBullet(Bullet* bullet) {
	EnemyBulletsList.push_back(bullet);
}

void BulletsManager::CreateBullet(MegamanBullet* megamanBullet) {
	MegamanBulletsList.push_back(megamanBullet);
}

void BulletsManager::UpdateBullets() {
	for (int i = 0; i < MegamanBulletsList.size(); i++) {
		if (MegamanBulletsList.at(i)->isDestroyed) {
			delete MegamanBulletsList.at(i);
			MegamanBulletsList.erase(MegamanBulletsList.begin() + i);
			i--;
		}
		else {
			MegamanBulletsList.at(i)->Update();
		}
	}
	for (int i = 0; i < EnemyBulletsList.size(); i++) {
		if (EnemyBulletsList.at(i)->isDestroyed) {
			Effects::CreateExplosion(EnemyBulletsList[i]->x, EnemyBulletsList[i]->y);
			delete EnemyBulletsList.at(i);
			EnemyBulletsList.erase(EnemyBulletsList.begin() + i);
			i--;
		}
		else {
			EnemyBulletsList.at(i)->Update();
		}
	}
}

void BulletsManager::RenderBullets() {
	for (int i = 0; i < MegamanBulletsList.size(); i++) {
		MegamanBulletsList.at(i)->Render();
	}
	for (int i = 0; i < EnemyBulletsList.size(); i++) {
		EnemyBulletsList.at(i)->Render();
	}
}