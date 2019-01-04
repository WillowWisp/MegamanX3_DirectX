#pragma once
#include "MObject.h"
#include "Effects.h"

#define BULLET_EXIST_TIME 300

class Bullet :
	public MObject
{
protected:
	
public:
	int dmg;
	bool isDestroyed;
	int state_t;

	Bullet();
	virtual ~Bullet();

	Bullet(int _x, int _y); //x,y tương đương vị trí của nòng súng
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void OnCollision(MObject *otherObj, char* sideCollided) = 0;
	void Destroyed();
};

