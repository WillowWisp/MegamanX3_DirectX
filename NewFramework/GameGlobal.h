#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include "Sound.h"

class GameGlobal
{
public:
	static int wndWidth, wndHeight; //size của cửa sổ

	static LPDIRECT3D9 d3d;
	static LPDIRECT3DDEVICE9 d3ddev; //device directX hiện tại
	static LPD3DXSPRITE mSpriteHandler; //spriteHanlder hiện tại

	static LPDIRECT3DSURFACE9 backbuffer;

	static bool isGameRunning; //trang thai xem game co dang chay khong

	GameGlobal();
	~GameGlobal();

	static bool IsIntersect(RECT rect1, RECT rect2);
};

