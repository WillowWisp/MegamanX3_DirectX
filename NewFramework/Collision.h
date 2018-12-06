#pragma once

#include <limits>
#include "MObject.h"

class Collision
{
public:
	Collision();
	~Collision();

	static bool AABBCheck(MObject object, MObject otherObject);
	static MObject GetSweptBroadphaseBox(MObject object);
	static float SweptAABB(MObject sprite1, MObject sprite2, float &normalx, float &normaly);
};

