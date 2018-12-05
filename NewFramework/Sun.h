#pragma once
#include "MObject.h"
#include <d3d9.h>
#include <d3dx9.h>
class Sun :
	public MObject
{
public:
	Sun(int X = 0, int Y = 0);
	~Sun();

	//
	void Update();
};

