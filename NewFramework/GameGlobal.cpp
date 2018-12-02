#include "GameGlobal.h"

int GameGlobal::wndWidth = 600;
int GameGlobal::wndHeight = 600;

LPDIRECT3D9 GameGlobal::d3d = NULL;
LPDIRECT3DDEVICE9 GameGlobal::d3ddev = NULL;
LPD3DXSPRITE GameGlobal::mSpriteHandler = NULL;

LPDIRECT3DSURFACE9 GameGlobal::backbuffer = NULL;

bool GameGlobal::isGameRunning = true;

GameGlobal::GameGlobal()
{
}


GameGlobal::~GameGlobal()
{
}
