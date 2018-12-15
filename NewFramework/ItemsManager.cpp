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

void ItemsManager::DropHPItem(int x, int y) {
	HPItemsList.push_back(new HP(x, y));
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
	//GAMELOG("HP: %d", HPItemsList.size());
}

void ItemsManager::RenderItems() {
	for (int i = 0; i < HPItemsList.size(); i++) {
		HPItemsList[i]->Render();
	}
}