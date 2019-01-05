#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "MObject.h"
#include "Megaman.h"
#include "GameLog.h"
#include "GameGlobal.h"
#include "EnemiesManager.h"

#define BOSS_SHURIKEIN 0
#define BOSS_BYTE 1
#define BOSS_BLAST_HORNET 2
#define BOSS_SHURIKEIN_DOOR 0
#define BOSS_BYTE_DOOR 2
#define BOSS_BLAST_HORNET_DOOR 5

class Events
{
public:
	static std::vector<MObject*> doorsList;
	static bool isOpeningDoor;
	static bool isFightingBoss;
	static int openingDoorId;
	static Megaman* megaman;
	static int event_t;

	Events();
	~Events();

	static void CreateDoor(int x, int y);
	static void OpenDoor(int doorId);
	static void PlayerDeath();
	static void RenderDoors();
	static void RenderEvents();
};

