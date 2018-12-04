#pragma once
#include"Scene.h"
#include"Graphics.h"
#include"Sprite.h"
#include"dxaudio.h"
class DemoScene : public Scene
{
public:
	//constructor
	DemoScene();
	//initialize scene
	void Start();

	//update
	void Update();

	//draw
	void Render();
	~DemoScene();

private:
	LPDIRECT3DSURFACE9 background;
	CSound *backgroundSound;
	Sprite *sprite;
};

