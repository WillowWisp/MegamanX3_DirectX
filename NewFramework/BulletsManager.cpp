#include "BulletsManager.h"

std::vector<Bullet*> BulletsManager::bulletsList;

BulletsManager::BulletsManager()
{
}


BulletsManager::~BulletsManager()
{
	for (int i = 0; i < bulletsList.size(); i++) {
		delete bulletsList[i];
	}
}

void BulletsManager::CreateBullet(Bullet* bullet) {
	bulletsList.push_back(bullet);
}

void BulletsManager::UpdateBullets() {
	for (int i = 0; i < bulletsList.size(); i++) {
		if (bulletsList[i]->isDestroyed) {
			delete bulletsList[i];
			bulletsList.erase(bulletsList.begin() + i);
			i--;
		}
		else {
			bulletsList[i]->Update();
		}
	}
}