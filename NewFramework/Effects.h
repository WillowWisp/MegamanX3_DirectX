#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "MObject.h"
#include "GameLog.h"

#define DEATH_ENERGY_EXIST_TIME 30
#define DEATH_ENERGY_SPEED 10 
#define SQUAREROOT_2 1.4

class Effects
{
public:
	static MObject* megamanEnergy;
	static std::vector<MObject*> explosionList;
	static std::vector<MObject*> smokeList;
	static std::vector<MObject*> megamanDeathEnergyList;
	static Sprite* screenFilter;
	Effects();
	~Effects();
	static void CreateMegamanEnergy();
	static void ResetMegamanEnergy();
	static void DrawMegamanEnergy(D3DXMATRIX matrix, int x, int y, int level);
	static void CreateMegamanDeathEnergy(int x, int y, int movex, int movey);
	static void CreateMegamanDeathEffect(int x, int y);
	static void DrawMegamanDeathEffect();
	static void CreateExplosion(int x, int y);
	static void DrawExplosions();
	static void CreateSmoke(int x, int y, int movey = 0);
	static void DrawSmokes();
	static void RenderEffects();
	static void CreateScreenFilter();
	static void DrawScreenFilter(D3DCOLOR color);
};

