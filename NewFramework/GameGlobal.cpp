#include "GameGlobal.h"

int GameGlobal::wndWidth = 800;
int GameGlobal::wndHeight = 640;

LPDIRECT3D9 GameGlobal::d3d = NULL;
LPDIRECT3DDEVICE9 GameGlobal::d3ddev = NULL;
LPD3DXSPRITE GameGlobal::mSpriteHandler = NULL;

LPDIRECT3DSURFACE9 GameGlobal::backbuffer = NULL;

bool GameGlobal::isGameRunning = true;

Camera *GameGlobal::camera = NULL;

GameGlobal::GameGlobal()
{
}


GameGlobal::~GameGlobal()
{
}

bool GameGlobal::IsIntersect(RECT rect1, RECT rect2) {
	if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top)
	{
		return false;
	}

	return true;
}

bool GameGlobal::IsContain(RECT container, RECT rect) {
	if (rect.left >= container.left && rect.right <= container.right && rect.top >= container.top && rect.bottom <= container.bottom)
	{
		return true;
	}

	return false;
}