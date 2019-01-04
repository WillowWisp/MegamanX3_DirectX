#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include "Sound.h"
#include "Camera.h"
#include <math.h>

class GameGlobal
{
public:
	static int wndWidth, wndHeight; //size của cửa sổ

	static LPDIRECT3D9 d3d;
	static LPDIRECT3DDEVICE9 d3ddev; //device directX hiện tại
	static LPD3DXSPRITE mSpriteHandler; //spriteHanlder hiện tại

	static LPDIRECT3DSURFACE9 backbuffer;

	static bool isGameRunning; //trang thai xem game co dang chay khong

	static Camera *camera;

	GameGlobal();
	~GameGlobal();

	static bool IsIntersect(RECT rect1, RECT rect2);
	static bool IsIntersectX(RECT rect1, RECT rect2);
	static bool IsIntersectY(RECT rect1, RECT rect2);
	static bool IsContain(RECT container, RECT rect);

	static float DistanceBetween(D3DXVECTOR2 point1, D3DXVECTOR2 point2);

	static int EaseInCirc(int currentTime, int startVal, int changeInVal, int duration);
	static int EaseOutCirc(int currentTime, int startVal, int changeInVal, int duration);
};

