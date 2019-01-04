#pragma once
#include "Scene.h"

class BlastHornetScene :
	public Scene
{
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

