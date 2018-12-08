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
	//GAMELOG("movey: %d", megaman->movey);;
	for (size_t i = 0; i < collisionList.size(); i++)
	{
		//char* isCollided = Collision::IsCollided(megaman, collisionList.at(i));
		//if (isCollided != (char*)"none")
		//{

		//	GAMELOG("ASD: %s", (char*)isCollided);
		//	//Gọi đến hàm OnCollision trong MObject
		//	megaman->OnCollision(collisionList.at(i));
		//	/*

		//	//goi den ham xu ly collision cua Player va MObject
		//	mPlayer->OnCollision(listCollision.at(i), r, sidePlayer);
		//	listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);
		//	}*/
		//}

		/*if (GameGlobal::IsIntersect(megaman->GetRect(), collisionList.at(i)->GetRect())) {
			GAMELOG("ASD: %d", megaman->GetRect().left);
			GAMELOG("ZXC: %d", collisionList.at(i)->GetRect().right);
			megaman->OnCollision(collisionList.at(i));
		}*/

		char* sideCollided = Collision::IsIntersect(megaman, collisionList.at(i));
		
		if (sideCollided != (char*)"none") {
			GAMELOG("Huong: %s", (char*)sideCollided);	
			megaman->OnCollision(collisionList.at(i), sideCollided);
		}
		else {
			count++;
		}
	}

	//if (count >= collisionList.size()) {
	//	megaman->isHitGround = false;
	//}
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
	/*if (Input::KeyDown(DIK_A)) {
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
	}*/
	megaman->SetWidthHeight();

	megaman->Upd();
	CheckCollision();
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


