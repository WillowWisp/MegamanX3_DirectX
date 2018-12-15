#include "Game.h"

Game::Game()
{
}


Game::~Game()
{
}

#pragma region Các khai báo cần thiết
//macros to read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HRESULT result;

//timing variable
long start = GetTickCount();

//auto device = GameGlobal::d3ddev;
LPD3DXSPRITE spriteHandler;

//Scene
#pragma endregion

LPDIRECT3DSURFACE9 background;
CSound *backgroundSound;
Sprite *sprite;

Sun* sun;
Sun* sun2;
Megaman* megaman;

NotorBanger* enemy;

GameMap *map;

std::vector<MObject*> collisionList;
DebugDraw *debugDraw;

int i = 0;


void DrawQuadtree(Quadtree *quadtree)
{
	D3DCOLOR oldColor = debugDraw->getColor();

	debugDraw->setColor(D3DCOLOR_XRGB(0, 0, 0));

	if (quadtree->nodes)
	{
		for (size_t i = 0; i < 4; i++)
		{
			DrawQuadtree(quadtree->nodes[i]);
		}
	}

	debugDraw->DrawRect(quadtree->region, GameGlobal::camera);

	if (quadtree->nodes)
	{
		for (int i = 0; i < 4; i++)
		{
			debugDraw->DrawRect(quadtree->nodes[i]->region, GameGlobal::camera);
		}
	}

	debugDraw->setColor(oldColor);
}

void DrawCollidable()
{
	D3DCOLOR oldColor = debugDraw->getColor();

	debugDraw->setColor(D3DCOLOR_XRGB(255, 255, 255));

	for (auto child : collisionList)
	{
		debugDraw->DrawRect(child->GetRect(), GameGlobal::camera);
	}

	debugDraw->setColor(oldColor);
}

void DrawBorders() {
	//DRAW WALL AND GROUND

	D3DCOLOR oldColor = debugDraw->getColor();

	RECT border;

	//left wall
	debugDraw->setColor(D3DCOLOR_XRGB(0, 255, 0));
	border.left = megaman->curLeftWallX;
	border.right = megaman->curLeftWallX + 2;
	border.top = 0;
	border.bottom = 10000;
	debugDraw->DrawRect(border, GameGlobal::camera);

	//right wall
	debugDraw->setColor(D3DCOLOR_XRGB(0, 0, 255));
	border.left = megaman->curRightWallX;
	border.right = megaman->curRightWallX + 2;
	border.top = 0;
	border.bottom = 10000;
	debugDraw->DrawRect(border, GameGlobal::camera);

	//ground
	debugDraw->setColor(D3DCOLOR_XRGB(0, 255, 255));
	border.left = 0;
	border.right = 10000;
	border.top = megaman->curGroundY;
	border.bottom = megaman->curGroundY + 2;
	debugDraw->DrawRect(border, GameGlobal::camera);

	//ceil
	debugDraw->setColor(D3DCOLOR_XRGB(255, 255, 0));
	border.left = 0;
	border.right = 10000;
	border.top = megaman->curCeilY;
	border.bottom = megaman->curCeilY + 2;
	debugDraw->DrawRect(border, GameGlobal::camera);

	debugDraw->setColor(oldColor);

	//END DRAW
}

void CheckCollision() {
	collisionList.clear();
	map->GetQuadtree()->GetObjectsCollidableWith(megaman, collisionList);
	//int count = 0;
	int newGroundY = 1000000;
	int newCeilY = -1000000;
	int newLeftWallX = -1000000;
	int newRightWallX = 1000000;
	bool isCollideStatic = false;

	//GAMELOG("movey: %d", megaman->movey);


	//if (megaman->movey == 0) {
	//	megaman->movey = 5;
	//}
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

			if (collisionList.at(i)->tag == (char*)"static") {	// Cham tuong, dat,..
				
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
					}
					else if (isCollided == (char*)"bottom") {
						megaman->curCeilY = collisionList.at(i)->y + collisionList.at(i)->height / 2;
						megaman->y = megaman->curCeilY + megaman->height / 2;
					}
					megaman->movey = 0;
				}
				else if (isCollided == (char*)"left" || isCollided == (char*)"right") {
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
			else {
				megaman->OnCollision(collisionList.at(i), (char*)isCollided);
			}

#pragma endregion
		}
		else {
			//count++;
			if (collisionList.at(i)->tag == (char*)"static") {
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
		}

		/*if (GameGlobal::IsIntersect(megaman->GetRect(), collisionList.at(i)->GetRect())) {
			GAMELOG("ASD: %d", megaman->GetRect().left);
			GAMELOG("ZXC: %d", collisionList.at(i)->GetRect().right);
			megaman->OnCollision(collisionList.at(i));
		}*/

	//	char* sideCollided = Collision::IsIntersect(megaman, collisionList.at(i));
	//	
	//	if (sideCollided != (char*)"none") {
	//		/*GAMELOG("Huong: %s", (char*)sideCollided);	*/
	//		megaman->OnCollision(collisionList.at(i), sideCollided);
	//		/*if (sideCollided == (char*)"top" || sideCollided == (char*)"unknown")
	//			collideTop = true;*/
	//		if (sideCollided == (char*)"left")
	//			collideLeft = true;
	//		if (sideCollided == (char*)"right")
	//			collideRight = true;
	//	}
	//	else
	//	{
	//		count++;
	//	}

	}
	/*megaman->isHitGround = collideTop ? collideTop : megaman->isHitGround;*/

	/*megaman->isHitGround = collideTop;*/
	////megaman->isHitWallLeft = collideLeft;
	////megaman->isHitWallRight = collideRight;

	if (!isCollideStatic) {
		/*megaman->isHitGround = false;
		megaman->isHitWallRight = false;
		megaman->isHitWallLeft = false;*/
		megaman->curGroundY = 100000;
		megaman->curCeilY = -100000;
		megaman->curLeftWallX = -100000;
		megaman->curRightWallX = 100000;
	}

	megaman->curGroundY = min(megaman->curGroundY, newGroundY);
	megaman->curCeilY = max(megaman->curCeilY, newCeilY);
	megaman->curLeftWallX = max(megaman->curLeftWallX, newLeftWallX);
	megaman->curRightWallX = min(megaman->curRightWallX, newRightWallX);

	//GAMELOG("%d", megaman->isHitGround);
}

void CheckCollisionEnemy() {
	if (enemy->isDestroyed)
		return;

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

	//Check collision with Megaman
	//enemy->MoveXYToCorner();
	//megaman->MoveXYToCorner();
	//char* isCollided = Collision::IsCollided(enemy, megaman);
	//enemy->MoveXYToCenter();
	//megaman->MoveXYToCenter();
	char* isCollided = Collision::IsIntersect(enemy, megaman);

	if (isCollided != (char*)"none") {
		enemy->OnCollision(megaman, isCollided);
		megaman->OnCollision(enemy, (char*)"X");
	}
}

void CheckCollisionItems() {
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
			//megaman->OnCollision(hp, (char*)"X");
		}
	}
}

void CheckCollisionBullets() {
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
		char* isCollided = Collision::IsCollided(bullet, megaman);
		bullet->MoveXYToCenter();
		megaman->MoveXYToCenter();

		if (isCollided != (char*)"none") {
			bullet->OnCollision(megaman, isCollided);
			megaman->TakeDmg(bullet->dmg);
		}

	}

	if (enemy->isDestroyed) //Temp
		return;

	for (auto bullet : BulletsManager::MegamanBulletsList) {
		//collisionList.clear();
		//map->GetQuadtree()->GetObjectsCollidableWith(bullet, collisionList);

		//for (size_t i = 0; i < collisionList.size(); i++) {
		//	bullet->MoveXYToCorner();
		//	collisionList.at(i)->MoveXYToCorner();
		//	char* isCollided = Collision::IsCollided(bullet, collisionList.at(i));
		//	bullet->MoveXYToCenter();
		//	collisionList.at(i)->MoveXYToCenter();

		//	if (isCollided != (char*)"none") {
		//		bullet->OnCollision(collisionList.at(i), isCollided);
		//	}
		//}

		bullet->MoveXYToCorner();
		bullet->SetSignedMoveX();
		enemy->MoveXYToCorner();
		char* isCollided = Collision::IsCollided(bullet, enemy);
		bullet->MoveXYToCenter();
		bullet->SetUnsignedMoveX();
		enemy->MoveXYToCenter();

		if (isCollided != (char*)"none") {
			enemy->TakeDmg(bullet->dmg);
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
				enemy->TakeDmg(bullet->dmg);
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

//Xử lý Init
void Start() {
	//background = Graphics::LoadSurface((char*)"myBackground.bmp");
	background = Graphics::LoadSurface((char*)"BG2.bmp");
	backgroundSound = Sound::LoadSound((char*)"bgmusic.wav");
	//Sound::PlaySoundA(backgroundSound);
	debugDraw = new DebugDraw();
	megaman = new Megaman();
	Random::Init();

	enemy = new NotorBanger(megaman);

	//hp = new HP(500, 500, 0);
	ItemsManager::DropItem(new HP(500, 500, 0));

	//map = new GameMap((char*)"Resources/test.tmx");
	map = new GameMap((char*)"Resources/test7.tmx");
	//map->GetQuadtree()->Insert(enemy);

	
	GameGlobal::camera = new Camera(GameGlobal::wndWidth, GameGlobal::wndHeight);
	//GameGlobal::camera->position = D3DXVECTOR3(0,0,0);
	
	map->SetCamera(GameGlobal::camera);
	megaman->SetCamera(GameGlobal::camera);
}

//Hàm này để xử lý logic mỗi frame
void UpdateCameraWorldMap()
{
	//khi megaman di chuyen qua window width /2 thi bat dau di chuyen camera ngang qua
	if (megaman->x > GameGlobal::camera->position.x)
		GameGlobal::camera->position = D3DXVECTOR3(megaman->x, GameGlobal::camera->position.y, 0);
	//khi vi tri megaman nho hon vi tri camera (nghia la da di sat mep ben trai map)
	if ((megaman->x < GameGlobal::camera->position.x))
		GameGlobal::camera->position = D3DXVECTOR3(GameGlobal::wndWidth / 2, map->GetHeight() - GameGlobal::wndHeight / 2, 0);
	//khi vi tri megaman toi vi tri tan cung cua map
	if ((megaman->x + GameGlobal::wndWidth / 2 >= map->GetWidth()))
		GameGlobal::camera->position = D3DXVECTOR3(map->GetWidth() - GameGlobal::wndWidth / 2, map->GetHeight() - GameGlobal::wndHeight / 2, 0);
	//khi vi tri megaman nhay cao hon vi tri window height / 2 thi bat dau di chuyen camera doc len
	if (megaman->y < GameGlobal::camera->position.y)
		GameGlobal::camera->position = D3DXVECTOR3(GameGlobal::camera->position.x, megaman->y, 0);
	//khi vi tri megaman rot xuong ( chua rot toi tan cung cua map)
	if (megaman->y >= GameGlobal::camera->position.y&&GameGlobal::camera->position.y != (map->GetHeight() - GameGlobal::wndHeight / 2))
		GameGlobal::camera->position = D3DXVECTOR3(GameGlobal::camera->position.x, megaman->y, 0);
	//khi rot toi vi tri tan cung cua map (nhung chua di het map ngang)
	if (megaman->x >= GameGlobal::camera->position.x&&megaman->y >= GameGlobal::camera->position.y&&GameGlobal::camera->position.y >= (map->GetHeight() - GameGlobal::wndHeight / 2) &&
		(GameGlobal::camera->position.x + GameGlobal::wndWidth / 2) != map->GetWidth())
		GameGlobal::camera->position = D3DXVECTOR3(megaman->x, map->GetHeight() - GameGlobal::wndHeight / 2, 0);
	

	/*if (GameGlobal::camera->GetBound().left < 0)
	{
		//vi position cua camera ma chinh giua camera
		//luc nay o vi tri goc ben trai cua the gioi thuc
		GameGlobal::camera->position = D3DXVECTOR3(GameGlobal::camera->width / 2, GameGlobal::camera->position.y, 0);
	}

	if (GameGlobal::camera->GetBound().right > map->GetWidth())
	{
		//luc nay cham goc ben phai cua the gioi thuc
		GameGlobal::camera->position = D3DXVECTOR3(map->GetWidth() - GameGlobal::camera->width / 2,
			GameGlobal::camera->position.y, 0);
	}

	if (GameGlobal::camera->GetBound().top < 0)
	{
		//luc nay cham goc tren the gioi thuc
		GameGlobal::camera->position = D3DXVECTOR3(GameGlobal::camera->position.x, GameGlobal::camera->height / 2, 0);
	}

	if (GameGlobal::camera->GetBound().bottom > map->GetHeight())
	{
		//luc nay cham day cua the gioi thuc
		GameGlobal::camera->position = D3DXVECTOR3(GameGlobal::camera->position.x,
			map->GetHeight() - GameGlobal::camera->height / 2, 0);
	}*/
}

void Update() {
	//if (Input::KeyDown(DIK_A)) {
	//	GameGlobal::camera->position.x -= 5;
	//}
	//if (Input::KeyDown(DIK_S)) {
	//	megaman->y += 5;
	//}
	/*
	if (Input::KeyDown(DIK_W)) {
		GameGlobal::camera->position.y -= 5;
	}
	if (Input::KeyDown(DIK_S)) {
		GameGlobal::camera->position.y += 5;
	}*/
	UpdateCameraWorldMap();
	//megaman->SetWidthHeight();

	if (!enemy->isDestroyed) {
		//map->GetQuadtree()->Remove(enemy);
		enemy->Updates();
		//map->GetQuadtree()->Insert(enemy);
	}

	BulletsManager::UpdateBullets();
	ItemsManager::UpdateItems();

	if (Input::KeyDown(DIK_C)) {
		ItemsManager::DropItem(new HP(megaman->x + 50, megaman->y, 0));
	}

	/*megaman->Update();*/
	
	if (Input::KeyDown(DIK_M)) {
		int count = 0;
		map->GetQuadtree()->Debug(count);
		GAMELOG("count: %d", count);
	}

	CheckCollisionEnemy();
	CheckCollisionItems();
	CheckCollisionBullets();
	CheckCollision();
}

//Hàm này để render lên màn hình
void Render() {
	GameGlobal::d3ddev->StretchRect(background, NULL, GameGlobal::backbuffer, NULL, D3DTEXF_NONE);

	//start sprite handler
	//spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	GameGlobal::d3ddev->StretchRect(background, NULL, GameGlobal::backbuffer, NULL, D3DTEXF_NONE);

	GameGlobal::mSpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	map->Draw();
	if (!enemy->isDestroyed) {
		enemy->Render();
	}
	//BulletsManager::UpdateBullets();
	ItemsManager::RenderItems();
	BulletsManager::RenderBullets();


	megaman->Update();
	megaman->Render();
	//GAMELOG("bullet count: %d", BulletsManager::MegamanBulletsList.size());

	debugDraw->DrawRect(megaman->GetRect(), GameGlobal::camera);

	DrawBorders();
	DrawQuadtree(map->GetQuadtree());
	DrawCollidable();

	GameGlobal::mSpriteHandler->End();

	//stop drawing
	//spriteHandler->End();
}


int Game::Game_Init(HWND hWnd) {
	//init keyboard
	if (!Input::InitKeyboard(hWnd)) {
		MessageBox(hWnd, "Error initializing the keyboard", "Error", MB_OK);
		return 0;
	}

	//init mouse
	/*if (!Input::InitMouse(hWnd)) {
		MessageBox(hWnd, "Error initializing the mouse", "Error", MB_OK);
		return 0;
	}*/

	//create sprite handler object
	
	result = D3DXCreateSprite(GameGlobal::d3ddev, &GameGlobal::mSpriteHandler);
	if (result != D3D_OK) {
		return 0;
	}
	//spriteHandler = GameGlobal::mSpriteHandler;
	
	Start();

	//return okay
	return 1;
}

void Game::Game_Run(HWND hWnd) {
	//make sure the Direct3D device is valid
	if (GameGlobal::d3ddev == NULL) {
		return;
	}

	//update mouse & keyboard
	//Input::PollMouse();
	Input::PollKeyboard();

	//---UPDATE PER FRAME---
	if (GetTickCount() - start >= UPDATE_RATE) {
		//reset timing
		start = GetTickCount();

		Update();

		GameGlobal::d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, RGB(255, 0, 255), 0.0f, 0);
		//start rendering
		if (GameGlobal::d3ddev->BeginScene()) {

			Render();

			//stop rendering
			GameGlobal::d3ddev->EndScene();
		}
	}
	//----------------


	//display the back buffer on the screen
	GameGlobal::d3ddev->Present(NULL, NULL, NULL, NULL);

	//check for escape key (to exit program)
	if (KEY_DOWN(VK_ESCAPE)) {
		PostMessage(hWnd, WM_DESTROY, 0, 0);
	}
}

void Game::Game_End(HWND hWnd) {
	//free all
	GameGlobal::mSpriteHandler->Release();
}


