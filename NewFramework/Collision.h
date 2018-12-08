#pragma once

#include <limits>
#include "MObject.h"

class Collision
{
public:
	Collision();
	~Collision();

	static bool AABBCheck(MObject *object, MObject *otherObject);
	static MObject* GetSweptBroadphaseBox(MObject *object);
	static bool SweptAABB(MObject *object, MObject *otherObject, float &normalx, float &normaly);
	static char* IsCollided(MObject *object, MObject *otherObj);
	static char* IsIntersect(MObject *obj1, MObject *obj2);
};

