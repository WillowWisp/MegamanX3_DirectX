#include "DemoScene.h"



DemoScene::DemoScene()
{

}

void DemoScene::Start()
{
	background = Graphics::LoadSurface((char*)"myBackground.bmp");
	backgroundSound = Sound::LoadSound((char*)"bgmusic.wav");
	//Sound::PlaySound(backgroundSound);
	sprite = new Sprite((char*)"BomberMan.bmp");
	sprite->position = D3DXVECTOR3(1, 1, 0);
}

void DemoScene::Update()
{
	Sound::LoopSound(backgroundSound);
}

void DemoScene::Render()
{
	GameGlobal::d3ddev->StretchRect(background, NULL, GameGlobal::backbuffer, NULL, D3DTEXF_NONE);

	GameGlobal::mSpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(D3DXVECTOR3(), RECT(), D3DXVECTOR2(5, 5), D3DXVECTOR2(150, 150));
	GameGlobal::mSpriteHandler->End();
}


DemoScene::~DemoScene()
{
}
