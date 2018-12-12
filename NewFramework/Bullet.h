#pragma once
#include "MObject.h"

class Bullet : public MObject
{
protected:
	int dmg;
public:
	Bullet();
	~Bullet();

	Bullet(int _x, int _y); //x,y tương đương vị trí của nòng súng
};

