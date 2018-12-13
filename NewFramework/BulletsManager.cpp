#include "BulletsManager.h"

std::vector<Bullet*> BulletsManager::bulletsList;

BulletsManager::BulletsManager()
{
}


BulletsManager::~BulletsManager()
{
}

void BulletsManager::CreateBullet(Bullet* bullet) {
	BulletsManager::bulletsList.push_back(bullet);
}

void BulletsManager::UpdateBullets() {
	for (int i = 0; i < bulletsList.size(); i++) {
		if (BulletsManager::bulletsList[i]->isDestroyed) {
			delete BulletsManager::bulletsList[i];
			BulletsManager::bulletsList.erase(bulletsList.begin() + i);
			i--;
		}
		else {
			BulletsManager::bulletsList[i]->Update();
		}
	}
}