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
	static bool SweptAABB(MObject object, MObject otherObject, float &normalx, float &normaly);
};

