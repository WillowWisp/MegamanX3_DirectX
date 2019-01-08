#include "Scene.h"
//CSound *explosion = Sound::LoadSound((char*)"Resources/Sounds/Effects/explosion.wav");

Scene::Scene()
{
	
}

void Scene::Start()
{
	
}

void Scene::Render()
{
}

void Scene::Update()
{
}

void Scene::CheckCollision() {
	if (Events::isOpeningDoor)
		return;
	collisionList.clear();
	map->GetQuadtree()->GetObjectsCollidableWith(megaman, collisionList);
	//int count = 0;
	int newGroundY = 1000000;
	int newCeilY = -1000000;
	int newLeftWallX = -1000000;
	int newRightWallX = 1000000;
	bool isCollideStatic = false;
	bool isOnSlope = false;

	//GAMELOG("movey: %d", megaman->movey);


	//if (megaman->movey == 0) {
	//	megaman->movey = 5;
	//}
	for (int i = 0; i < map->slopes.size(); i++) {
		if ((GameGlobal::IsIntersect(megaman->GetRect(), map->slopes[i])
			|| GameGlobal::IsContain(map->slopes[i], megaman->GetRect()))
			&& (megaman->y + megaman->height / 2) < map->slopes[i].bottom)
		{
			isOnSlope = true;
		}
	}

	for (size_t i = 0; i < collisionList.size(); i++)
	{
		megaman->MoveXYToCorner();
		collisionList.at(i)->MoveXYToCorner();
		megaman->SetSignedMoveX();
		char* isCollided = Collision::IsCollided(megaman, collisionList.at(i));
		megaman->MoveXYToCenter();
		collisionList.at(i)->MoveXYToCenter();
		megaman->SetUnsignedMoveX();
		if (isCollided != (char*)"none")
		{
#pragma region Megaman Collision
			//GAMELOG("ASD: %s", (char*)isCollided);
			//Gọi đến hàm OnCollision trong MObject

			if (collisionList.at(i)->tag == (char*)"static"
				|| collisionList.at(i)->tag == (char*)"elevator") {	// Cham tuong, dat,..

																	/*
																	//goi den ham xu ly collision cua Player va MObject
																	mPlayer->OnCollision(listCollision.at(i), r, sidePlayer);
																	listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);
																	}*/
				isCollideStatic = true;
				if (isCollided == (char*)"top" || isCollided == (char*)"bottom") {
					if (isCollided == (char*)"top") {
						megaman->curGroundY = collisionList.at(i)->y - collisionList.at(i)->height / 2;
						megaman->y = megaman->curGroundY - megaman->height / 2;
						//megaman->delta_t = 0;
						if (collisionList.at(i)->tag == (char*)"elevator") {
							MovingObjects::elevator->Start();
						}
					}
					else if (isCollided == (char*)"bottom") {
						megaman->curCeilY = collisionList.at(i)->y + collisionList.at(i)->height / 2;
						megaman->y = megaman->curCeilY + megaman->height / 2;
					}
					megaman->movey = 0;
				}
				else if (isCollided == (char*)"left" || isCollided == (char*)"right") {

					if (isOnSlope) {
						megaman->curGroundY = collisionList.at(i)->y - collisionList.at(i)->height / 2;
						megaman->y = megaman->curGroundY - megaman->height / 2 + 4;
						megaman->movey = 0;

						if (isCollided == (char*)"left") {
							megaman->curRightWallX = collisionList.at(i)->x;
							//megaman->x = megaman->curRightWallX - megaman->width / 2;
						}
						else if (isCollided == (char*)"right") {
							megaman->curLeftWallX = collisionList.at(i)->x;
							//megaman->x = megaman->curLeftWallX + megaman->width / 2;
						}
						//megaman->movex = 0;
					}
					else {
						if (isCollided == (char*)"left") {
							megaman->curRightWallX = collisionList.at(i)->x - collisionList.at(i)->width / 2;
							megaman->x = megaman->curRightWallX - megaman->width / 2;
						}
						else if (isCollided == (char*)"right") {
							megaman->curLeftWallX = collisionList.at(i)->x + collisionList.at(i)->width / 2;
							megaman->x = megaman->curLeftWallX + megaman->width / 2;
						}
						megaman->movex = 0;
					}
				}
				/*if (collisionList.at(i)->tag == (char*)"elevator") {
				MovingObjects::elevator->Start();
				}*/
			}
			else {
				megaman->OnCollision(collisionList.at(i), (char*)isCollided);
			}

#pragma endregion
		}
		else {
			//count++;
			if (false) {
				//megaman->y = megaman->curGroundY - megaman->height / 2 + 4;
			}
			else {
				if (collisionList.at(i)->tag == (char*)"static"
					|| collisionList.at(i)->tag == (char*)"elevator") {
					if (GameGlobal::IsIntersectX(megaman->GetRect(), collisionList.at(i)->GetRect())
						&& megaman->y <= collisionList.at(i)->y - collisionList.at(i)->height / 2) {
						newGroundY = min(newGroundY, collisionList.at(i)->y - collisionList.at(i)->height / 2);
					}
					if (GameGlobal::IsIntersectX(megaman->GetRect(), collisionList.at(i)->GetRect())
						&& megaman->y >= collisionList.at(i)->y + collisionList.at(i)->height / 2) {
						newCeilY = max(newCeilY, collisionList.at(i)->y + collisionList.at(i)->height / 2);
					}
					if (GameGlobal::IsIntersectY(megaman->GetRect(), collisionList.at(i)->GetRect())
						&& megaman->x <= collisionList.at(i)->x - collisionList.at(i)->width / 2) {
						newRightWallX = min(newRightWallX, collisionList.at(i)->x - collisionList.at(i)->width / 2);
					}
					if (GameGlobal::IsIntersectY(megaman->GetRect(), collisionList.at(i)->GetRect())
						&& megaman->x >= collisionList.at(i)->x + collisionList.at(i)->width / 2) {
						newLeftWallX = max(newLeftWallX, collisionList.at(i)->x + collisionList.at(i)->width / 2);
					}
				}
				else if (collisionList.at(i)->tag == (char*)"deathTrap") {
					if (GameGlobal::IsIntersect(megaman->GetRect(), collisionList.at(i)->GetRect())) {
						megaman->OnCollision(collisionList.at(i), (char*)"X");
					}
				}
			}
		}

	}

	if (!Events::isOpeningDoor || !Events::isFightingBoss) {
		for (int i = 0; i < Events::doorsList.size(); i++) {
			if (Events::isOpeningDoor)
				break;

			megaman->MoveXYToCorner();
			Events::doorsList[i]->MoveXYToCorner();
			megaman->SetSignedMoveX();
			char* isCollided = Collision::IsCollided(megaman, Events::doorsList[i]);
			megaman->MoveXYToCenter();
			Events::doorsList[i]->MoveXYToCenter();
			megaman->SetUnsignedMoveX();
			char* isIntersect = Collision::IsIntersect(megaman, Events::doorsList[i]);
			if (isCollided != (char*)"none" || isIntersect != (char*)"none") {
				if (Events::isFightingBoss
					|| Events::doorsList[i]->anim->curframe != 0)
				{

				}
				else {
					Events::OpenDoor(i);
				}

				break;
			}
		}
	}

	if (!isCollideStatic) {
		megaman->curGroundY = 100000;
		megaman->curCeilY = -100000;
		megaman->curLeftWallX = -100000;
		megaman->curRightWallX = 100000;
	}

	megaman->curGroundY = min(megaman->curGroundY, newGroundY);
	megaman->curCeilY = max(megaman->curCeilY, newCeilY);
	megaman->curLeftWallX = max(megaman->curLeftWallX, newLeftWallX);
	megaman->curRightWallX = min(megaman->curRightWallX, newRightWallX);

	if (isOnSlope) {
		megaman->curLeftWallX = -1000000;
		megaman->curRightWallX = 1000000;
	}

	megaman->isOnSlope = isOnSlope;

	if (Events::isFightingBoss) {
		if (Events::openingDoorId + 1 < Events::doorsList.size()) {
			//lock boss room
			megaman->curRightWallX = Events::doorsList[Events::openingDoorId + 1]->x
				- Events::doorsList[Events::openingDoorId + 1]->width - 5;
		}
	}

}

void Scene::CheckCollisionEnemy() {
	
	if (!Events::isFightingBoss) {
		for (auto enemy : EnemiesManager::enemiesList) {
			collisionList.clear();
			map->GetQuadtree()->GetObjectsCollidableWith(enemy, collisionList);

			for (size_t i = 0; i < collisionList.size(); i++) {
				enemy->MoveXYToCorner();
				collisionList.at(i)->MoveXYToCorner();
				char* isCollided = Collision::IsCollided(enemy, collisionList.at(i));
				enemy->MoveXYToCenter();
				collisionList.at(i)->MoveXYToCenter();

				if (isCollided != (char*)"none") {
					enemy->OnCollision(collisionList.at(i), isCollided);
				}
			}

			char* isCollided = Collision::IsIntersect(enemy, megaman);

			if (isCollided != (char*)"none") {
				enemy->OnCollision(megaman, isCollided);
				//megaman->OnCollision(enemy, (char*)"X");
				if (enemy->tag == "boss") {
					megaman->TakeDmg(4);
				}
				else {
					megaman->TakeDmg(2);
				}
			}
		}
	}
	else {
		Enemy* enemy = EnemiesManager::boss;
		collisionList.clear();
		map->GetQuadtree()->GetObjectsCollidableWith(enemy, collisionList);

		for (size_t i = 0; i < collisionList.size(); i++) {
			enemy->MoveXYToCorner();
			if (enemy->tag == (char*)"bossByte")
				enemy->SetSignedMoveX();
			collisionList.at(i)->MoveXYToCorner();
			char* isCollided = Collision::IsCollided(enemy, collisionList.at(i));
			if (enemy->tag == (char*)"bossByte")
				enemy->SetUnsignedMoveX();
			enemy->MoveXYToCenter();
			collisionList.at(i)->MoveXYToCenter();

			if (isCollided != (char*)"none") {
				enemy->OnCollision(collisionList.at(i), isCollided);
			}
		}

		//Check collision with Megaman
		enemy->MoveXYToCorner();
		if (enemy->tag == (char*)"bossByte")
			enemy->SetSignedMoveX();
		megaman->MoveXYToCorner();
		char* isCollided = Collision::IsCollided(enemy, megaman);
		enemy->MoveXYToCenter();
		if (enemy->tag == (char*)"bossByte")
			enemy->SetUnsignedMoveX();
		megaman->MoveXYToCenter();

		if (isCollided != (char*)"none") {
			enemy->OnCollision(megaman, isCollided);
		}
		else {
			char* isCollided = Collision::IsIntersect(enemy, megaman);

			if (isCollided != (char*)"none") {
				enemy->OnCollision(megaman, isCollided);
			}
		}
	}
}

void Scene::CheckCollisionItems() {
	for (auto hp : ItemsManager::HPItemsList) {
		collisionList.clear();
		map->GetQuadtree()->GetObjectsCollidableWith(hp, collisionList);

		for (size_t i = 0; i < collisionList.size(); i++) {
			hp->MoveXYToCorner();
			collisionList.at(i)->MoveXYToCorner();
			char* isCollided = Collision::IsCollided(hp, collisionList.at(i));
			hp->MoveXYToCenter();
			collisionList.at(i)->MoveXYToCenter();

			if (isCollided != (char*)"none") {
				hp->OnCollision(collisionList.at(i), isCollided);
			}
		}

		char* isCollided = Collision::IsIntersect(hp, megaman);

		if (isCollided != (char*)"none") {
			if (!hp->isDisappeared)
				megaman->Heal(hp->heal);
			hp->OnCollision(megaman, isCollided);
		}
	}
}

void Scene::CheckCollisionBullets() {
	for (auto bullet : BulletsManager::EnemyBulletsList) {
		collisionList.clear();
		map->GetQuadtree()->GetObjectsCollidableWith(bullet, collisionList);

		for (size_t i = 0; i < collisionList.size(); i++) {
			bullet->MoveXYToCorner();
			collisionList.at(i)->MoveXYToCorner();
			char* isCollided = Collision::IsCollided(bullet, collisionList.at(i));
			bullet->MoveXYToCenter();
			collisionList.at(i)->MoveXYToCenter();

			if (isCollided != (char*)"none") {
				bullet->OnCollision(collisionList.at(i), isCollided);
			}
		}

		//char* isCollided = Collision::IsIntersect(bullet, megaman);

		//if (isCollided != (char*)"none") {
		//	if (!bullet->isDestroyed)
		//		megaman->Heal(hp->heal);
		//	hp->OnCollision(megaman, isCollided);
		//	//megaman->OnCollision(hp, (char*)"X");
		//}

		bullet->MoveXYToCorner();
		megaman->MoveXYToCorner();
		char* isCollided = Collision::IsIntersect(bullet, megaman);
		bullet->MoveXYToCenter();
		megaman->MoveXYToCenter();

		if (isCollided != (char*)"none") {
			bullet->OnCollision(megaman, isCollided);
			megaman->TakeDmg(bullet->dmg);
		}

	}

	for (auto bullet : BulletsManager::MegamanBulletsList) {

		if (!Events::isFightingBoss) {
			for (auto enemy : EnemiesManager::enemiesList) {

				bullet->MoveXYToCorner();
				bullet->SetSignedMoveX();
				enemy->MoveXYToCorner();
				char* isCollided = Collision::IsCollided(bullet, enemy);
				bullet->MoveXYToCenter();
				bullet->SetUnsignedMoveX();
				enemy->MoveXYToCenter();

				if (isCollided != (char*)"none") {
					enemy->TakeDmg(bullet->dmg);
					//enemy->OnCollision(bullet, (char*)"X");
					if (!enemy->isDestroyed) {
						bullet->Vanish();
					}
					else {
						//map->GetQuadtree()->Remove(enemy);
						Sound::PlaySoundA(explosion);
					}
				}
				else {
					char* isCollided = Collision::IsIntersect(bullet, enemy);

					if (isCollided != (char*)"none") {
						enemy->TakeDmg(bullet->dmg);
						//enemy->OnCollision(bullet, (char*)"X");
						if (!enemy->isDestroyed) {
							bullet->Vanish();
						}
						else {
							//map->GetQuadtree()->Remove(enemy);
							Sound::PlaySoundA(explosion);
						}
					}
				}
			}
		}
		else {
			Enemy* enemy = EnemiesManager::boss;

			bullet->MoveXYToCorner();
			bullet->SetSignedMoveX();
			enemy->MoveXYToCorner();
			char* isCollided = Collision::IsCollided(bullet, enemy);
			bullet->MoveXYToCenter();
			bullet->SetUnsignedMoveX();
			enemy->MoveXYToCenter();

			if (isCollided != (char*)"none") {
				//enemy->TakeDmg(bullet->dmg);
				enemy->OnCollision(bullet, (char*)"X");
				if (!enemy->isDestroyed) {
					bullet->Vanish();
				}
				else {
					//map->GetQuadtree()->Remove(enemy);
				}
			}
			else {
				char* isCollided = Collision::IsIntersect(bullet, enemy);

				if (isCollided != (char*)"none") {
					//enemy->TakeDmg(bullet->dmg);
					enemy->OnCollision(bullet, (char*)"X");
					if (!enemy->isDestroyed) {
						bullet->Vanish();
					}
					else {
						//map->GetQuadtree()->Remove(enemy);
					}
				}
			}
		}
	}

	for (auto megamanBullet : BulletsManager::MegamanBulletsList) {
		for (auto enemyBullet : BulletsManager::EnemyBulletsList) {
			char* isCollided = Collision::IsIntersect(megamanBullet, enemyBullet);
			if (isCollided != (char*)"none") {
				enemyBullet->OnCollision(megamanBullet, isCollided);
				if (megamanBullet->tag != (char*)"megamanBulletLvl2") {
					megamanBullet->Vanish();
				}
			}
		}
	}
}

//Hàm này để xử lý logic mỗi frame
void Scene::UpdateCameraWorldMap()
{
	if (megaman->isDead || (megaman->state == STATE_INTRO && megaman->isRespawned))
		return;
	if (!Events::isOpeningDoor) {
		GameGlobal::camera->Reposition(megaman->GetRect());
		EnemiesManager::SpawnEnemiesNearCamera(megaman);
	}
	else {
		GameGlobal::camera->TransitionToBossRoom();
	}
}

void Scene::RepositionCameraToSpawnSpot() {
	UI::DeleteBossHPBar();
	UI::HPBar->anim->ChangeAnimFrames(MEGAMAN_MAX_HP, MEGAMAN_MAX_HP);
	UI::curHP = MEGAMAN_MAX_HP;
	GameGlobal::camera->cameraBorders.clear();
	GameGlobal::camera->cameraBorders = map->cameraBorders;
	while (GameGlobal::camera->cameraBorders.size() > 0
		&& !GameGlobal::IsContain(GameGlobal::camera->cameraBorders[0], respawnSpots[curPlayerRespawnSpot])) {
		//erase until spawn spots in camera
		GameGlobal::camera->cameraBorders.erase(GameGlobal::camera->cameraBorders.begin());
	}
	GameGlobal::camera->Reposition(respawnSpots[curPlayerRespawnSpot]);
}

void Scene::RespawnPlayer() {
	RECT respawnRect = respawnSpots[curPlayerRespawnSpot];
	megaman->Respawn(respawnRect.left + (respawnRect.right - respawnRect.left) / 2,
					respawnRect.top + (respawnRect.bottom - respawnRect.top) / 2 - GameGlobal::camera->height / 2);
	for (int i = 0; i < Events::doorsList.size(); i++) {
		if (Events::doorsList[i]->x > megaman->x) {
			Events::doorsList[i]->anim->ChangeAnimFrames(0, 0);
		}
	}
	if (Events::isFightingBoss) {
		EnemiesManager::boss->isDestroyed = true;
		if (Events::openingDoorId == BOSS_BLAST_HORNET_DOOR) {
			Events::openingDoorId--;
		}
	}
}

Scene::~Scene()
{
	delete megaman;
	delete map;
	delete backgroundSound;
	delete sprite;
	for (int i = 0; i < collisionList.size(); i++) {
		delete collisionList[i];
	}
}
