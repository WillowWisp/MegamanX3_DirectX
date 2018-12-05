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
Megaman* megaman;

//Xử lý Init
void Start() {
	background = Graphics::LoadSurface((char*)"myBackground.bmp");
	backgroundSound = Sound::LoadSound((char*)"bgmusic.wav");
	//Sound::PlaySound(backgroundSound);
	sprite = new Sprite((char*)"BomberMan.bmp");
	sprite->position = D3DXVECTOR3(300, 300, 0);

	sun = new Sun(100, 100);
	megaman = new Megaman();
}

//Hàm này để xử lý logic mỗi frame
void Update() {
	//Sound::LoopSound(backgroundSound);
}

//Hàm này để render lên màn hình
void Render() {
	//start sprite handler
	//spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	GameGlobal::d3ddev->StretchRect(background, NULL, GameGlobal::backbuffer, NULL, D3DTEXF_NONE);

	GameGlobal::mSpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(D3DXVECTOR3(), RECT(), D3DXVECTOR2(5, 5), D3DXVECTOR2(150, 150));
	//sprite->Draw(D3DXVECTOR3());

	sun->Update();
	megaman->Update();

	GameGlobal::mSpriteHandler->End();

	//stop drawing
	//spriteHandler->End();
}


int Game::Game_Init(HWND hWnd) {
	//branch
	Init_DirectInput(hWnd);
	Init_Keyboard(hWnd);
	Init_Mouse(hWnd);

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

	//poll DirectInput devices
	Poll_Keyboard();
	Poll_Mouse();

	//---UPDATE PER FRAME---
	if (GetTickCount() - start >= 10) {
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