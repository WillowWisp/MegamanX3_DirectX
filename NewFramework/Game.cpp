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

//LPDIRECT3DSURFACE9 background;
//CSound *backgroundSound;
//Sprite *sprite;
//
//Sun* sun;
//Sun* sun2;
Megaman* megaman;
//
//GameMap *map;
//
std::vector<MObject*> collisionList;
DebugDraw *debugDraw;

int i = 0;

Scene* blastHornetScene = new BlastHornetScene();

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

void DrawCameraRegion()
{
	D3DCOLOR oldColor = debugDraw->getColor();

	debugDraw->setColor(D3DCOLOR_XRGB(255, 0, 0));

	for (auto child : GameGlobal::camera->cameraBorders)
	{
		debugDraw->DrawRect(child, GameGlobal::camera);
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


//Tam thoi
//void CheckCollisionShurikein() {
//	collisionList.clear();
//	map->GetQuadtree()->GetObjectsCollidableWith(shurikein, collisionList);
//
//	for (size_t i = 0; i < collisionList.size(); i++) {
//		shurikein->MoveXYToCorner();
//		collisionList.at(i)->MoveXYToCorner();
//		char* isCollided = Collision::IsCollided(shurikein, collisionList.at(i));
//		shurikein->MoveXYToCenter();
//		collisionList.at(i)->MoveXYToCenter();
//
//		if (isCollided != (char*)"none") {
//			shurikein->OnCollision(collisionList.at(i), isCollided);
//		}
//	}
//}

//Xử lý Init
void Start() {
	debugDraw = new DebugDraw();
	Random::Init();

	blastHornetScene->Start();

}


void Update() {

	blastHornetScene->Update();
}

//Hàm này để render lên màn hình
void Render() {

	blastHornetScene->Render();
}


int Game::Game_Init(HWND hWnd) {
	//init keyboard
	if (!Input::InitKeyboard(hWnd)) {
		MessageBox(hWnd, "Error initializing the keyboard", "Error", MB_OK);
		return 0;
	}

	//init_sound
	if (!Sound::Init_DirectSound(hWnd)) {
		MessageBox(hWnd, "Error initializing DirectSound", "Error", MB_OK);
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


