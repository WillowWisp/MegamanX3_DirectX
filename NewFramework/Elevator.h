#pragma once
#include "MObject.h"

#define ELEVATOR_SPEED 1

class Elevator :
	public MObject
{
public:
	int spawnHeight;
	int destHeight;
	int initHeight;
	bool isStarted;
	bool isSpawning;
	bool isDoneMoving;
	Elevator();
	Elevator(int _x, int _y, int _spawnHeight, int _desHeight);
	~Elevator();
	void OnCollision(MObject *otherObj, char* sideCollided);
	void Start();
	void Respawn();
	void Update();
	void Render();
};

