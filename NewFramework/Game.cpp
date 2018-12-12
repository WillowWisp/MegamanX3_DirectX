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

GameMap *map;

std::vector<MObject*> collisionList;
DebugDraw *debugDraw;

int i = 0;

void CheckCollision() {
	collisionList.clear();
	map->GetQuadtree()->GetObjectsCollidableWith(megaman, collisionList);
	int count = 0;
	bool collideTop = false;
	bool collideLeft = false;
	bool collideRight = false;
	int newGroundY = 1000000;
	int newCeilY = -1000000;
	int newLeftWallX = -1000000;
	int newRightWallX = 1000000;

	//GAMELOG("movey: %d", megaman->movey);


	//if (megaman->movey == 0) {
	//	megaman->movey = 5;
	//}

	for (size_t i = 0; i < collisionList.size(); i++)
	{
		megaman->MoveXYToCorner();
		collisionList.at(i)->MoveXYToCorner();
		char* isCollided = Collision::IsCollided(megaman, collisionList.at(i));
		megaman->MoveXYToCenter();
		collisionList.at(i)->MoveXYToCenter();
		if (isCollided != (char*)"none")
		{

			//GAMELOG("ASD: %s", (char*)isCollided);
			//Gọi đến hàm OnCollision trong MObject
			megaman->OnCollision(collisionList.at(i), (char*)isCollided);
			/*

			//goi den ham xu ly collision cua Player va MObject
			mPlayer->OnCollision(listCollision.at(i), r, sidePlayer);
			listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);
			}*/
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
			//else if ((isCollided == (char*)"left" || isCollided == (char*)"right") 
			//	&& GameGlobal::IsIntersectY(megaman->GetRect(), collisionList.at(i)->GetRect())) {
			//	if (megaman->dirRight > 0) {
			//		megaman->curRightWallX = collisionList.at(i)->x - collisionList.at(i)->width / 2;
			//	}
			//	else {
			//		megaman->curLeftWallX = collisionList.at(i)->x + collisionList.at(i)->width / 2;
			//	}
			//}
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

			collideTop = true;
		}
		else {
			count++;
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

	if (count >= collisionList.size()) {
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

//Xử lý Init
void Start() {
	background = Graphics::LoadSurface((char*)"myBackground.bmp");
	backgroundSound = Sound::LoadSound((char*)"bgmusic.wav");
	//Sound::PlaySoundA(backgroundSound);
	debugDraw = new DebugDraw();
	megaman = new Megaman();

	map = new GameMap((char*)"Resources/test.tmx");
	
	GameGlobal::camera = new Camera(GameGlobal::wndWidth, GameGlobal::wndHeight);
	GameGlobal::camera->position = D3DXVECTOR3(GameGlobal::wndWidth / 2, map->GetHeight() - GameGlobal::wndHeight / 2, 0);
	
	map->SetCamera(GameGlobal::camera);
}

//Hàm này để xử lý logic mỗi frame
void Update() {
	if (Input::KeyDown(DIK_A)) {
		GameGlobal::camera->position.x -= 5;
	}
	if (Input::KeyDown(DIK_D)) {
		GameGlobal::camera->position.x += 5;
	}
	if (Input::KeyDown(DIK_W)) {
		GameGlobal::camera->position.y -= 5;
	}
	if (Input::KeyDown(DIK_S)) {
		GameGlobal::camera->position.y += 5;
	}
	megaman->SetWidthHeight();

	megaman->Upd();
	CheckCollision();
	//GAMELOG("%d", megaman->isHitGround);
}

void DrawQuadtree(Quadtree *quadtree)
{
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
}

void DrawCollidable()
{
	for (auto child : collisionList)
	{
		debugDraw->DrawRect(child->GetRect(), GameGlobal::camera);
	}
}


//Hàm này để render lên màn hình
void Render() {

	GameGlobal::d3ddev->StretchRect(background, NULL, GameGlobal::backbuffer, NULL, D3DTEXF_NONE);
	
	//start sprite handler
	//spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	GameGlobal::d3ddev->StretchRect(background, NULL, GameGlobal::backbuffer, NULL, D3DTEXF_NONE);

	GameGlobal::mSpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	map->Draw();
	megaman->Update();

	debugDraw->DrawRect(megaman->GetRect(), GameGlobal::camera);

	//DRAW WALL AND GROUND

	D3DCOLOR oldColor = debugDraw->getColor();

	RECT border;

	//left wall
	debugDraw->setColor(D3DCOLOR_XRGB(0, 255, 0));
	border.left = megaman->curLeftWallX;
	border.right = megaman->curLeftWallX + 2;
	border.top = 0;
	border.bottom = 1000;
	debugDraw->DrawRect(border, GameGlobal::camera);

	//right wall
	debugDraw->setColor(D3DCOLOR_XRGB(0, 0, 255));
	border.left = megaman->curRightWallX;
	border.right = megaman->curRightWallX + 2;
	border.top = 0;
	border.bottom = 1000;
	debugDraw->DrawRect(border, GameGlobal::camera);

	//ground
	debugDraw->setColor(D3DCOLOR_XRGB(0, 255, 255));
	border.left = 0;
	border.right = 1000;
	border.top = megaman->curGroundY;
	border.bottom = megaman->curGroundY + 2;
	debugDraw->DrawRect(border, GameGlobal::camera);

	//ceil
	debugDraw->setColor(D3DCOLOR_XRGB(255, 255, 0));
	border.left = 0;
	border.right = 1000;
	border.top = megaman->curCeilY;
	border.bottom = megaman->curCeilY + 2;
	debugDraw->DrawRect(border, GameGlobal::camera);

	debugDraw->setColor(oldColor);

	//END DRAW

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
	if (!Input::InitMouse(hWnd)) {
		MessageBox(hWnd, "Error initializing the mouse", "Error", MB_OK);
		return 0;
	}

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


