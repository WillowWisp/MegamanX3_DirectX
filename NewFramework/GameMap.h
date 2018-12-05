#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#include "Sprite.h"
#include "./MapReader/Tmx.h.in"
#include "GameGlobal.h"
#include "Camera.h"

class GameMap
{
	Tmx::Map* map;
	std::map<int, Sprite*> listTileset;
	/* ví dụ:
	map<char*, int> mymap;
	mymap["a"] = 0;
	mymap["b"] = 3;
	*/
	Camera *camera;

	void LoadMap(char* filePath);
public:
	GameMap();
	~GameMap();

	GameMap(char* filePath);

	Tmx::Map* GetMap();

	int GetWidth();
	int GetHeight();
	int GetTileWidth();
	int GetTileHeight();

	void SetCamera(Camera *_camera);

	void Draw();
};

