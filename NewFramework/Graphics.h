#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "GameGlobal.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	static int Init_Direct3D(HWND, int, int);
	static LPDIRECT3DSURFACE9 LoadSurface(char*);
	static LPDIRECT3DTEXTURE9 LoadTexture(char*, D3DCOLOR);
};

