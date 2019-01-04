#include "GameGlobal.h"

int GameGlobal::wndWidth = 508;
int GameGlobal::wndHeight = 440;

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

bool GameGlobal::IsIntersectX(RECT rect1, RECT rect2) {
	return (rect1.left < rect2.right && rect2.left < rect1.right);
}

bool GameGlobal::IsIntersectY(RECT rect1, RECT rect2) {
	//if (IsIntersect(rect1, rect2))
	//	return false;
	//if (rect1.left >= rect2.left && rect1.right <= rect2.right)
	//	return false;
	//if (rect1.left < rect2.left) {
	//
	//}
	//else if (rect)
	return (rect1.bottom - 5 > rect2.top && rect2.bottom > rect1.top);
}

bool GameGlobal::IsContain(RECT container, RECT rect) {
	if (rect.left >= container.left && rect.right <= container.right && rect.top >= container.top && rect.bottom <= container.bottom)
	{
		return true;
	}

	return false;
}

float GameGlobal::DistanceBetween(D3DXVECTOR2 point1, D3DXVECTOR2 point2) {
	return sqrt((point2.x - point1.x)*(point2.x - point1.x) + (point2.y - point1.y)*(point2.y - point1.y));
}

int GameGlobal::EaseInCirc(int currentTime, int startVal, int changeInVal, int duration) {
	currentTime /= duration;
	return -changeInVal * (sqrt(1 - currentTime * currentTime) - 1) + startVal;
}

int GameGlobal::EaseOutCirc(int currentTime, int startVal, int changeInVal, int duration) {
	currentTime /= duration;
	currentTime--;
	return changeInVal * sqrt(1 - currentTime * currentTime) + startVal;
}