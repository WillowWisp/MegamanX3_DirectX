#pragma once
#include "Elevator.h"

class MovingObjects
{
public:
	static Elevator* elevator;
	static bool isSpawnedElevator;

	MovingObjects();
	~MovingObjects();

	static void CreateElevator(int x, int y, int firstDest, int secondDest);
	static void UpdateElevator();
	static void SetElevatorSizeAsMovingSize(); //for quadtree
	static void SetElevatorNormalSize();
	static void UpdateMovingObjects();
	static void RenderMovingObjects();
};

