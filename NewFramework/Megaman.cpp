#include "Megaman.h"

#define _CRT_SECURE_NO_WARNINGS

Megaman::Megaman()
{
	x = 100;
	y = 380;
	/*width = 20;
	height = 25;*/
	movex = MEGAMAN_SPEED;
	movey = 0;
	dirUp = 1;
	dirRight = 1;
	anim = new Animation(60, 7, 9, ANIM_DELAY + 10);

	char s[50];
	for (int i = 0; i < 60; i++) {
		sprintf_s(s, "sprites/megaman/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
	setState(STATE_IDLE);
}


Megaman::~Megaman()
{
}

void Megaman::setState(int newState)
{
	state = newState;
	switch (newState)
	{
	case STATE_IDLE:
		setAnimState(7, 9, ANIM_DELAY + 10);
		break;
	case STATE_RUNNING:
		setAnimState(13, 23, ANIM_DELAY);
		break;
	case STATE_SHOOTING:
		setAnimState(11, 12, ANIM_DELAY);
		break;
	case STATE_RUNNING_N_SHOOTING:
		setAnimState(24, 33, ANIM_DELAY);
		break;
	default:
		setAnimState(7, 10, ANIM_DELAY);
		break;
	}
}

void Megaman::update()
{
	if (Key_Down(DIK_Z) && !(Key_Down(DIK_LEFT) || ((Key_Down(DIK_RIGHT))))) {
		if (stateChanged(STATE_SHOOTING))
			setState(STATE_SHOOTING);
	}
	else if (Key_Down(DIK_Z) && Key_Down(DIK_LEFT)) {
		if (stateChanged(STATE_RUNNING_N_SHOOTING))
			setState(STATE_RUNNING_N_SHOOTING);

		x -= MEGAMAN_SPEED;

		if (horizontalDirChanged(-1))
			changeDirHorizontal();
	}
	else if (Key_Down(DIK_Z) && Key_Down(DIK_RIGHT)) {
		if (stateChanged(STATE_RUNNING_N_SHOOTING))
			setState(STATE_RUNNING_N_SHOOTING);

		x += MEGAMAN_SPEED;

		if (horizontalDirChanged(1))
			changeDirHorizontal();
	}
	else if (Key_Down(DIK_LEFT)) {
		if (stateChanged(STATE_RUNNING))
			setState(STATE_RUNNING);

		x -= MEGAMAN_SPEED;

		if (horizontalDirChanged(-1))
			changeDirHorizontal();
	}
	else if (Key_Down(DIK_RIGHT)) {
		if (stateChanged(STATE_RUNNING))
			setState(STATE_RUNNING);

		x += MEGAMAN_SPEED;

		if (horizontalDirChanged(1))
			changeDirHorizontal();
	}
	else {
		if (stateChanged(STATE_IDLE))
			setState(STATE_IDLE);
	}

	D3DXVECTOR2 translation = D3DXVECTOR2(x, y);
	D3DXVECTOR2 scale = D3DXVECTOR2(2 * dirRight, 2);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &translation);
	MObject::update();
}
