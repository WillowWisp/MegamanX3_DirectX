#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "MObject.h"
class Effects
{
public:
	static MObject* megamanEnergy;
	Effects();
	~Effects();
	static void CreateMegamanEnergy();
	static void DrawMegamanEnergy(D3DXMATRIX matrix, int x, int y, int level);
};

