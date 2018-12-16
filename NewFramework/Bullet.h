#pragma once
#include "MObject.h"
class Bullet :
	public MObject
{
protected:
	int dmg;
public:
	bool isDestroyed;
	Bullet();
	~Bullet();

	Bullet(int _x, int _y); //x,y tương đương vị trí của nòng súng
	virtual void Update() = 0;
	virtual void Render();
};

