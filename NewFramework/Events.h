#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "MObject.h"
#include "Megaman.h"
#include "GameLog.h"
#include "GameGlobal.h"

class Events
{
public:
	static std::vector<MObject*> doorsList;
	static bool isOpeningDoor;
	static bool isFightingBoss;
	static int openingDoorId;
	static Megaman* megaman;

	Events();
	~Events();

	static void CreateDoor(int x, int y);
	static void OpenDoor(int doorId);
	static void RenderDoors();
	static void RenderEvents();
};

