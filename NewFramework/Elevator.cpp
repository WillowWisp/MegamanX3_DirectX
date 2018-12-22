#include "Elevator.h"

Elevator::Elevator()
{

}


Elevator::~Elevator()
{
}

Elevator::Elevator(int _x, int _y, int _spawnHeight, int _destHeight)
{
	x = _x;
	y = _y;
	spawnHeight = _spawnHeight;
	destHeight = _destHeight;
	initHeight = _y;
	movex = 0;
	movey = -ELEVATOR_SPEED;
	dirUp = 1;
	dirRight = 1;
	isStarted = false;
	isSpawning = true;
	isDoneMoving = false;

	tag = (char*)"elevator";

	anim = new Animation(3, 0, 2, -1);
	char s[50];
	for (int i = 0; i < 3; i++) {
		sprintf_s(s, "sprites/elevator/resized/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
	SetWidthHeight();
}

void Elevator::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == (char*)"megaman") {
		if (!isStarted) {
			Start();
		}
	}
}

void Elevator::Start() {
	if (isDoneMoving)
		return;
	isStarted = true;
	movey = -ELEVATOR_SPEED;
	anim->ChangeAnimFrames(0, 2);
}

void Elevator::Respawn() {
	y = initHeight;
	movex = 0;
	movey = -ELEVATOR_SPEED;
	dirUp = 1;
	dirRight = 1;
	isStarted = false;
	isSpawning = true;
}

void Elevator::Update() {
	if (isDoneMoving)
		return;

	if (isSpawning) {
		if (y <= spawnHeight) {
			anim->ChangeAnimFrames(0, 0);
			anim->animcount = 0;
			isSpawning = false;
			movey = 0;
		}
		else {
			movey = -ELEVATOR_SPEED;
		}
	}
	else {
		if (!isStarted)
			return;
		if (y <= destHeight) {
			anim->ChangeAnimFrames(0, 0);
			anim->animcount = 0;
			isStarted = false;
			isDoneMoving = true;
			movey = 0;
		}
	}
	MObject::Update();
}

void Elevator::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex * dirRight, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1.5);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	MObject::Render();
}

