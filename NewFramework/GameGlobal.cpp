#include "GameGlobal.h"

int GameGlobal::wndWidth = 800;
int GameGlobal::wndHeight = 800;

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
