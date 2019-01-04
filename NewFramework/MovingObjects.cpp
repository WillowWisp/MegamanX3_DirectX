#include "MovingObjects.h"

Elevator* MovingObjects::elevator;
bool MovingObjects::isSpawnedElevator = false;


MovingObjects::MovingObjects()
{
}


MovingObjects::~MovingObjects()
{
	delete elevator;
}

void MovingObjects::CreateElevator(int x, int y, int firstDest, int secondDest) {
	elevator = new Elevator(x, y, firstDest, secondDest);
}

void MovingObjects::SetElevatorSizeAsMovingSize() {
	elevator->height = elevator->initHeight - elevator->destHeight + elevator->height;
	elevator->y = (elevator->initHeight + elevator->destHeight) / 2;
}

void MovingObjects::SetElevatorNormalSize() {
	elevator->SetWidthHeight();
	elevator->height -= 10;
	elevator->y = elevator->initHeight;
}

void MovingObjects::UpdateElevator() {
	if (elevator->isDoneMoving)
		return;

	RECT cameraRect = GameGlobal::camera->GetBound();
	RECT outerCameraRect; //a rect surrounding cameraRect, enemies spawn in this region
	outerCameraRect.left = cameraRect.left - OUTER_CAMERA_ZONE;
	outerCameraRect.right = cameraRect.right + OUTER_CAMERA_ZONE;
	outerCameraRect.top = cameraRect.top - OUTER_CAMERA_ZONE;
	outerCameraRect.bottom = cameraRect.bottom + OUTER_CAMERA_ZONE;

	if (elevator->isStarted && !GameGlobal::IsContain(outerCameraRect, elevator->GetRect())) {
		elevator->Respawn();
	}
	elevator->Update();
}

void MovingObjects::UpdateMovingObjects() {
	UpdateElevator();
}

void MovingObjects::RenderMovingObjects() {
	elevator->Render();
}
