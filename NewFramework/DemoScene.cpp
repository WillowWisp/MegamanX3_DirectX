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
	//test for update game
	sprite->position= D3DXVECTOR3(sprite->position.x+5, 1, 0);

	if(sprite->position.x>=GameGlobal::wndWidth-sprite->width)
		sprite->position= D3DXVECTOR3(1, 1, 0);

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
