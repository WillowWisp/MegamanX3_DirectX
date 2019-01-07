#pragma once
#include"Graphics.h"
#include"Sprite.h"
#include"dxaudio.h"
#include "Megaman.h"
#include "UI.h"
#include "GameMap.h"
#include "MovingObjects.h"
#include "Collision.h"

class Scene
{
protected:
	Megaman * megaman;
	GameMap *map;
	std::vector<MObject*> collisionList;
	std::vector<RECT> respawnSpots;
	LPDIRECT3DSURFACE9 background;
	
	Sprite *sprite;
	int curPlayerRespawnSpot;
	int scene_t;

	void CheckCollision();
	void CheckCollisionEnemy();
	void CheckCollisionItems();
	void CheckCollisionBullets();

	CSound *explosion;
	CSound *backgroundSound;
	CSound *boss;
	CSound *stageclear;

public:
	Scene();
	//ham khoi tao scene
	virtual void Start();

	//ham ve scene
	virtual void Render();

	//ham update
	virtual void Update();
	void UpdateCameraWorldMap();

	void RepositionCameraToSpawnSpot();
	void RespawnPlayer();
	virtual ~Scene();
};

