#pragma once

#include <d3d9.h>
#include <stdio.h>
#include "GameGlobal.h"
#include "Graphics.h"
#include "Sound.h"
#include "Sprite.h"
#include"SceneManager.h"
#include"Scene.h"
#include"DemoScene.h"
#include "Sun.h"
#include "Megaman.h"

class Game
{
public:
	Game();
	~Game();

	static int Game_Init(HWND);
	static void Game_Run(HWND);
	static void Game_End(HWND);
};

