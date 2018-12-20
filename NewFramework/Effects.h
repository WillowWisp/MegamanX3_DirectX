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
	static std::vector<MObject*> smokeList;
	Effects();
	~Effects();
	static void CreateMegamanEnergy();
	static void ResetMegamanEnergy();
	static void DrawMegamanEnergy(D3DXMATRIX matrix, int x, int y, int level);
	static void CreateExplosion(int x, int y);
	static void DrawExplosions();
	static void CreateSmoke(int x, int y, int movey = 0);
	static void DrawSmokes();
	static void RenderEffects();
};

