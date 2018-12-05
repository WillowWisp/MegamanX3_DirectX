#include "Megaman.h"

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
	SetState(STATE_IDLE);
}


Megaman::~Megaman()
{
}

void Megaman::SetState(int newState)
{
	state = newState;
	switch (newState)
	{
	case STATE_IDLE:
		SetAnimState(7, 9, ANIM_DELAY + 10);
		break;
	case STATE_RUNNING:
		SetAnimState(13, 23, ANIM_DELAY);
		break;
	case STATE_SHOOTING:
		SetAnimState(11, 12, ANIM_DELAY);
		break;
	case STATE_RUNNING_N_SHOOTING:
		SetAnimState(24, 33, ANIM_DELAY);
		break;
	default:
		SetAnimState(7, 10, ANIM_DELAY);
		break;
	}
}

void Megaman::Update()
{
	if (Key_Down(DIK_Z) && !(Key_Down(DIK_LEFT) || ((Key_Down(DIK_RIGHT))))) {
		if (StateChanged(STATE_SHOOTING))
			SetState(STATE_SHOOTING);
	}
	else if (Key_Down(DIK_Z) && Key_Down(DIK_LEFT)) {
		if (StateChanged(STATE_RUNNING_N_SHOOTING))
			SetState(STATE_RUNNING_N_SHOOTING);

		x -= MEGAMAN_SPEED;

		if (HorizontalDirChanged(-1))
			ChangeDirHorizontal();
	}
	else if (Key_Down(DIK_Z) && Key_Down(DIK_RIGHT)) {
		if (StateChanged(STATE_RUNNING_N_SHOOTING))
			SetState(STATE_RUNNING_N_SHOOTING);

		x += MEGAMAN_SPEED;

		if (HorizontalDirChanged(1))
			ChangeDirHorizontal();
	}
	else if (Key_Down(DIK_LEFT)) {
		if (StateChanged(STATE_RUNNING))
			SetState(STATE_RUNNING);

		x -= MEGAMAN_SPEED;

		if (HorizontalDirChanged(-1))
			ChangeDirHorizontal();
	}
	else if (Key_Down(DIK_RIGHT)) {
		if (StateChanged(STATE_RUNNING))
			SetState(STATE_RUNNING);

		x += MEGAMAN_SPEED;

		if (HorizontalDirChanged(1))
			ChangeDirHorizontal();
	}
	else {
		if (StateChanged(STATE_IDLE))
			SetState(STATE_IDLE);
	}

	D3DXVECTOR2 translation = D3DXVECTOR2(x, y);
	D3DXVECTOR2 scale = D3DXVECTOR2(2 * dirRight, 2);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &translation);
	MObject::Update();
}
