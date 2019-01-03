#pragma once
#include "MObject.h"
#include "Effects.h"

#define MINE_SPEED 8

class ByteMine :
	public MObject
{
public:
	int destX;

	ByteMine();
	~ByteMine();

	ByteMine(int _x, int _y, int _dirRight);

	void Update();
	void Render();
	void Explode();
};

