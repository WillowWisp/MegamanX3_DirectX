#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "MObject.h"
#include "GameLog.h"

class Effects
{
public:
	static MObject* megamanEnergy;
	static std::vector<MObject*> explosionList;
	Effects();
	~Effects();
	static void CreateMegamanEnergy();
	static void DrawMegamanEnergy(D3DXMATRIX matrix, int x, int y, int level);
	static void CreateExplosion(int x, int y);
	static void DrawExplosions();
	static void RenderEffects();
};

