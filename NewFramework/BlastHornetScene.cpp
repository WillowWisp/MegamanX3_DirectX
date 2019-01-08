#include "BlastHornetScene.h"

//flag to recognize boss scene
bool flag = false;
bool isDie = false;

BlastHornetScene::BlastHornetScene()
{
}

void BlastHornetScene::Start()
{

	//background = Graphics::LoadSurface((char*)"myBackground.bmp");
	background = Graphics::LoadSurface((char*)"BG1.bmp");
	backgroundSound = Sound::LoadSound((char*)"Resources/Sounds/Musics/stage.wav");
	stageclear = Sound::LoadSound((char*)"Resources/Sounds/Musics/stage_clear.wav");
	explosion = Sound::LoadSound((char*)"Resources/Sounds/Effects/explosion.wav");
	boss = Sound::LoadSound((char*)"Resources/Sounds/Musics/blast_hornet.wav");
	//Sound::PlaySoundA(backgroundSound);
	isBackgroundChanged = false;
	backgroundChangeX = 5300;

	megaman = new Megaman(133, 1600); //Start
	//megaman = new Megaman(3250, 1530); //Bug
	//megaman = new Megaman(4500, 2200); //Shurikein
	//megaman = new Megaman(11000, 2318);	//Byte
	//megaman = new Megaman(15040, 3800);	//Blast Hornet


	UI::InitHPBar();
	Events::megaman = megaman;


	//ItemsManager::DropItem(new HP(500, 500, 0));
	MovingObjects::CreateElevator(1792, 2114, 1910, 900);

	map = new GameMap((char*)"Resources/map_big.tmx");

	MovingObjects::SetElevatorSizeAsMovingSize();
	map->GetQuadtree()->Insert(MovingObjects::elevator);
	MovingObjects::SetElevatorNormalSize();

	GameGlobal::camera = new Camera(GameGlobal::wndWidth, GameGlobal::wndHeight);
	//GameGlobal::camera->position = D3DXVECTOR3(0,0,0);
	GameGlobal::camera->cameraBorders = map->cameraBorders;
	respawnSpots = map->respawnSpots;

	map->SetCamera(GameGlobal::camera);
	megaman->SetCamera(GameGlobal::camera);

	curPlayerRespawnSpot = 0;
	scene_t = -1;
	Effects::CreateScreenFilter();
}

void BlastHornetScene::Update()
{
	if (flag == false) Sound::LoopSound(backgroundSound);
	/*if (megaman->isDead && megaman->forcedAnim_t > DYING_TIME * 2) {
		RespawnPlayer();
		return;
	}*/

	if (megaman->isDead) {
		
	}

	UpdateCameraWorldMap();


	if (Events::isOpeningDoor) {
		Events::OpenDoor(Events::openingDoorId);
	}

	if (Events::isFightingBoss == false)
	{
		flag = false;
		Sound::StopSound(boss);
		if (Events::openingDoorId == BOSS_BLAST_HORNET_DOOR 
			&& Events::isOpeningDoor==false
			) // when megaman win (openingdoor = blast hornet && no event opening door (while fighting) )
		{
			Sound::StopSound(backgroundSound);
			flag = true;
			//regconize that blast hornet is died
			if (isDie == false)
			{
				isDie = true;
				if (isDie == true)
				{
					Sound::PlaySoundA(stageclear); //play once time

				}
			}
			
		}
	}
	
	if (Events::isFightingBoss == true)
	{
		flag = true;
		if (Events::openingDoorId == BOSS_BLAST_HORNET_DOOR) // when megaman fighting with blast hornet, play other sound
		{
			Sound::StopSound(backgroundSound);
			Sound::StopSound(stageclear);
			Sound::LoopSound(boss);
		}


	}
	MovingObjects::UpdateMovingObjects();


	EnemiesManager::UpdateEnemies();

	BulletsManager::UpdateBullets();
	ItemsManager::UpdateItems();

	/*if (Input::KeyDown(DIK_C)) {
	ItemsManager::DropItem(new HP(megaman->x + 50, megaman->y, 0));
	}
	if (Input::KeyDown(DIK_B)) {
	Events::isFightingBoss = false;
	}
	if (Input::KeyDown(DIK_N)) {
	Events::isFightingBoss = true;
	}
	if (Input::KeyDown(DIK_E)) {
	//EnemiesManager::SpawnEnemy(megaman, megaman->x - 100, megaman->y - 50);
	GAMELOG("enemies: %d", EnemiesManager::enemiesList.size());
	}*/

	CheckCollisionEnemy();
	CheckCollisionItems();
	CheckCollisionBullets();
	CheckCollision();

	if (curPlayerRespawnSpot + 1 < respawnSpots.size()) {
		if (megaman->x > respawnSpots[curPlayerRespawnSpot + 1].left) {
			curPlayerRespawnSpot += 1;
		}
	}

	//Change Background;
	if (megaman->x > backgroundChangeX && !isBackgroundChanged) {
		background = Graphics::LoadSurface((char*)"BG2.bmp");
		isBackgroundChanged = true;
	}

}

void BlastHornetScene::Render()
{
	//GameGlobal::d3ddev->StretchRect(background, NULL, GameGlobal::backbuffer, NULL, D3DTEXF_NONE);

	//start sprite handler
	//spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	GameGlobal::d3ddev->StretchRect(background, NULL, GameGlobal::backbuffer, NULL, D3DTEXF_NONE);

	GameGlobal::mSpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	map->Draw();

	Events::RenderEvents();

	EnemiesManager::RenderEnemies();
	ItemsManager::RenderItems();
	BulletsManager::RenderBullets();
	Effects::RenderEffects();

	megaman->Update();

	if (!megaman->isDead) {
		megaman->Render();
	}
	MovingObjects::RenderMovingObjects();
	UI::RenderUI();

	//Screen fade effect
	if (megaman->isDead 
		&& megaman->forcedAnim_t > DYING_TIME * 2) 
	{
		int t_start = megaman->forcedAnim_t - DYING_TIME * 2;
		int t_color = (255 / 5) + DYING_TIME * 2 + 1;
		int colorValue = 255 - ((megaman->forcedAnim_t - t_color) * 5);
		int t_respawn = t_color + 255 / 5;

		if (t_start * 5 <= 255) {
			Effects::DrawScreenFilter(D3DCOLOR_ARGB(t_start * 5, 255, 255, 255));
		}
		else if (colorValue >= 0) {
			if (colorValue == 0) {
				//RespawnPlayer();
				RepositionCameraToSpawnSpot();
			}
			Effects::DrawScreenFilter(D3DCOLOR_ARGB(255, colorValue, colorValue, colorValue));
		}
		else {
			if (255 + colorValue + 5 <= 0) {
				RespawnPlayer();
			}
			else {
				Effects::DrawScreenFilter(D3DCOLOR_ARGB(255 + colorValue + 5, 0, 0, 0));
			}
			/*GAMELOG("%d", colorValue);*/
		}
		GAMELOG("%d %d", colorValue, megaman->forcedAnim_t);
	}

	//stop drawing
	GameGlobal::mSpriteHandler->End();
}


BlastHornetScene::~BlastHornetScene()
{
}
