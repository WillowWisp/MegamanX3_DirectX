#include "NotorBanger.h"



NotorBanger::NotorBanger(MObject* _player)
{
	player = _player;

	x = 500;
	y = 500;
	movex = 0;
	movey = 0;
	dirUp = 1;
	dirRight = -1;

	delta_t = 0;

	anim = new Animation(19, 0, 0, ANIM_DELAY);
	char s[50];
	for (int i = 0; i < 19; i++) {
		sprintf_s(s, "sprites/notor_banger/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
}


NotorBanger::~NotorBanger()
{
}

void NotorBanger::OnCollision(MObject *otherObj, char* sideCollided) {

}

void NotorBanger::SetState(int newState) {
	state = newState;

	switch (newState) {
	case STATE_IDLE:
		SetAnimState(0, 0, ANIM_DELAY);
		break;
	case STATE_PREPARE:
		SetAnimState(0, 2, ANIM_DELAY);
		break;
	case STATE_JUMP:
		SetAnimState(3, 3, ANIM_DELAY);
		break;
	case STATE_45_UP:
		SetAnimState(4, 6, ANIM_DELAY);
		break;
	case STATE_90_UP:
		SetAnimState(4, 8, ANIM_DELAY);
		break;
	case STATE_45_DOWN:
		SetAnimState(10, 12, ANIM_DELAY);
		break;
	case STATE_90_DOWN:
		SetAnimState(8, 12, ANIM_DELAY);
		break;
	case STATE_45_SHOOT:
		SetAnimState(14, 15, ANIM_DELAY + 1);
		break;
	case STATE_90_SHOOT:
		SetAnimState(17, 18, ANIM_DELAY);
		break;
	}
}

void NotorBanger::Update() {
	delta_t++;

	//Nếu player ở tầm xa thì bắn 45 độ
	if (delta_t == 5) {		//Giơ nòng súng lên
		SetState(STATE_45_UP);
	}
	if (delta_t == 50) {	//Bắn
		SetState(STATE_45_SHOOT);
	}
	if (delta_t == 75) {	//Bắn
		SetState(STATE_45_SHOOT);
	}
	if (delta_t == 125) {	//Hạ nòng
		SetState(STATE_45_DOWN);
	}
	if (delta_t == 175) delta_t = 0;
}

void NotorBanger::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 translate = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + translate;

	D3DXVECTOR2 scale = D3DXVECTOR2(2 * dirRight, 2);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, NULL, NULL,
		NULL, &combined);
	anim->AnimateWithoutLoop(matrix);
}