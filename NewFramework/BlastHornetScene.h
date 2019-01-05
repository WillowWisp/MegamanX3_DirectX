#pragma once
#include "Scene.h"

class BlastHornetScene :
	public Scene
{
private:
	bool isBackgroundChanged;
	int backgroundChangeX;
public:
	BlastHornetScene();

	//initialize scene
	void Start();

	//update
	void Update();

	//draw
	void Render();

	~BlastHornetScene();
	
};

