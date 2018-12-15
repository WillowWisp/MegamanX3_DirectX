#include "HP.h"



HP::HP()
{

}

HP::HP(int _x, int _y, int _level) {
	x = _x;
	y = _y;
	level = _level;
	//isUsed = false;
	movex = 0;
	movey = 0;
	tag = (char*)"hpItem";
	
	if (level == 0) {
		level = Random::RandInt(1, 3);
	}

	switch (level)
	{
	case 1:
		anim = new Animation(8, 0, 0, HP_ANIM_DELAY - 1);
		heal = HEAL_AMOUNT_SMALL;
		break;
	case 2:
		anim = new Animation(8, 4, 4, HP_ANIM_DELAY - 1);
		heal = HEAL_AMOUNT_BIG;
		break;
	default:
		anim = new Animation(8, 0, 0, HP_ANIM_DELAY - 1);
		heal = HEAL_AMOUNT_SMALL;
		break;
	}

	char s[50];
	for (int i = 0; i < 8; i++) {
		sprintf_s(s, "sprites/items/%d.png", i);
		//sprintf_s(s, "sprites/megaman/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}

}


HP::~HP()
{
}

void HP::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == (char*)"static") {
		if (sideCollided == "top") {
			movex = 0;
			movey = 0;
			y = otherObj->y - otherObj->height / 2 - height / 2;
			SetState(HP_STATE_IDLE);
		}
	}
	else if (otherObj->tag == (char*)"megaman") {
		Disappear();
	}
}

void HP::SetState(int newState) {
	state = newState;
	state_t = -1;
	if (state == HP_STATE_DROPPING) {
		switch (level)
		{
		case 1:
			SetAnimState(0, 0, HP_ANIM_DELAY - 1);
			break;
		case 2:
			SetAnimState(4, 4, HP_ANIM_DELAY - 1);
			break;
		default:
			SetAnimState(0, 0, HP_ANIM_DELAY - 1);
			break;
		}
	}
	else if (state == HP_STATE_IDLE) {
		switch (level)
		{
		case 1:
			SetAnimState(1, 3, HP_ANIM_DELAY - 1);
			break;
		case 2:
			SetAnimState(5, 7, HP_ANIM_DELAY - 1);
			break;
		default:
			SetAnimState(1, 3, HP_ANIM_DELAY - 1);
			break;
		}
	}
	else if (state == HP_STATE_DISAPPEARING) {
		switch (level)
		{
		case 1:
			SetAnimState(1, 3, HP_ANIM_DELAY - 1);
			break;
		case 2:
			SetAnimState(5, 7, HP_ANIM_DELAY - 1);
			break;
		default:
			SetAnimState(1, 3, HP_ANIM_DELAY - 1);
			break;
		}
	}
}

void HP::Update() {
	if (isDisappeared)
		return;
	if (state == HP_STATE_DROPPING) {
		movey += 1;
	}
	else if (state == HP_STATE_IDLE && state_t >= HP_IDLE_ANIM_TIME) {
		SetState(HP_STATE_DISAPPEARING);
	}
	else if (state == HP_STATE_DISAPPEARING) {
		if (state_t >= HP_DISAPPEARING_ANIM_TIME) {
			Disappear();
			return;
		}
		else {
			if (state_t % 3 == 0) {
				color = D3DCOLOR_ARGB(0, 255, 255, 255);
			}
			else {
				color = D3DCOLOR_ARGB(180, 255, 255, 255);
			}
		}
	}
	state_t++;

	MObject::Update();
}

void HP::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x, y);
	D3DXVECTOR2 translate = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + translate;
	D3DXVECTOR2 scale = D3DXVECTOR2(2, 2);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);

	MObject::Render();
}

void HP::Disappear() {
	isDisappeared = true;
}
