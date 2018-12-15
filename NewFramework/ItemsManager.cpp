#include "ItemsManager.h"

std::vector<HP*> ItemsManager::HPItemsList;

ItemsManager::ItemsManager()
{
}


ItemsManager::~ItemsManager()
{
	for (int i = 0; i < HPItemsList.size(); i++) {
		delete HPItemsList[i];
	}
}

void ItemsManager::DropItem(HP* item) {
	HPItemsList.push_back(item);
}

void ItemsManager::UpdateItems() {
	for (int i = 0; i < HPItemsList.size(); i++) {
		if (HPItemsList[i]->isDisappeared) {
			delete HPItemsList[i];
			HPItemsList.erase(HPItemsList.begin() + i);
			i--;
		}
		else {
			HPItemsList[i]->Update();
		}
	}
}