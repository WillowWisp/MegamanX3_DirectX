#pragma once
#include "MObject.h"
#include "ItemsManager.h"
class Enemy :
	public MObject
{
protected:
	int HP;
public:
	int id;
	bool isDestroyed;

	Enemy();
	virtual ~Enemy();

	virtual void OnCollision(MObject *otherObj, char* sideCollided) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	void TakeDmg(int damage);
	void Destroyed();
};

