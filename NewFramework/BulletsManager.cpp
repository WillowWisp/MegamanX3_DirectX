#include "BulletsManager.h"

std::vector<MegamanBullet*> BulletsManager::MegamanBulletsList;
std::vector<Bullet*> BulletsManager::EnemyBulletsList;

BulletsManager::BulletsManager()
{
}


BulletsManager::~BulletsManager()
{
	for (int i = 0; i < MegamanBulletsList.size(); i++) {
		delete MegamanBulletsList[i];
	}
	for (int i = 0; i < EnemyBulletsList.size(); i++) {
		delete EnemyBulletsList[i];
	}
}

void BulletsManager::CreateBullet(Bullet* bullet) {
	EnemyBulletsList.push_back(bullet);
}

void BulletsManager::CreateBullet(MegamanBullet* megamanBullet) {
	MegamanBulletsList.push_back(megamanBullet);
}

void BulletsManager::UpdateBullets() {
	RECT cameraRect = GameGlobal::camera->GetBound();

	for (int i = 0; i < MegamanBulletsList.size(); i++) {
		if (MegamanBulletsList[i]->isDestroyed) {
			delete MegamanBulletsList[i];
			MegamanBulletsList.erase(MegamanBulletsList.begin() + i);
			i--;
		}
		else {
			MegamanBulletsList[i]->Update();
		}
	}
	for (int i = 0; i < EnemyBulletsList.size(); i++) {
		if (EnemyBulletsList[i]->isDestroyed) {
			Effects::CreateExplosion(EnemyBulletsList[i]->x, EnemyBulletsList[i]->y);
			delete EnemyBulletsList[i];
			EnemyBulletsList.erase(EnemyBulletsList.begin() + i);
			i--;
		}
		else if (!GameGlobal::IsContain(cameraRect, EnemyBulletsList[i]->GetRect())
			&& !GameGlobal::IsIntersect(cameraRect, EnemyBulletsList[i]->GetRect())) {
			delete EnemyBulletsList[i];
			EnemyBulletsList.erase(EnemyBulletsList.begin() + i);
			i--;
		}
		else {
			EnemyBulletsList[i]->Update();
		}
	}
}

void BulletsManager::RenderBullets() {
	for (int i = 0; i < MegamanBulletsList.size(); i++) {
		MegamanBulletsList[i]->Render();
	}
	for (int i = 0; i < EnemyBulletsList.size(); i++) {
		EnemyBulletsList[i]->Render();
	}
}