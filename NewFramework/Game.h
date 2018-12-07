#pragma once

#include <d3d9.h>
#include <stdio.h>
#include <vector>
#include "GameGlobal.h"
#include "Graphics.h"
#include "Sound.h"
#include "Sprite.h"
#include "Input.h"
#include "Collision.h"
#include "GameMap.h"
#include"SceneManager.h"
#include"Scene.h"
#include"DemoScene.h"
#include "Sun.h"
#include "Megaman.h"
#include "Camera.h"
#include "GameLog.h"

#define UPDATE_RATE 10

class Game
{
public:
	Game();
	~Game();

	static int Game_Init(HWND);
	static void Game_Run(HWND);
	static void Game_End(HWND);
};

