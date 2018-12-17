#include "Shurikein.h"



Shurikein::Shurikein(MObject* _player, int _x, int _y, int _dirRight)
{
	player = _player;
	x = _x;
	y = _y;
	dirRight = _dirRight;
	dirUp = 1;
	movex = 0;
	movey = 0;
	delta_t = 0;

	phase = 0;
	phase1Count = 3;
	phase2Count = 1;
	phase3Count = 7;

	anim = new Animation(106, 0, 41, ANIM_DELAY);
	char s[50];
	for (int i = 0; i < 106; i++) {
		sprintf_s(s, "sprites/shurikein/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
}


Shurikein::~Shurikein()
{
}

void Shurikein::SetState(int newState) {
	state = newState;
	switch (newState) {
	case STATE_APPEARING:
		SetAnimState(0, 41, ANIM_DELAY);
		break;
	case STATE_PHASE1_ROLL:
		phase = 1;
		SetAnimState(84, 105, ANIM_DELAY);
		break;
	case STATE_PHASE1_JUMP:
		//SetAnimState(0, 0, ANIM_DELAY);
		break;
	case STATE_PHASE2:
		phase = 2;
		SetAnimState(41, 51, ANIM_DELAY);
		break;
	case STATE_PHASE3:
		phase = 3;
		SetAnimState(52, 83, ANIM_DELAY);
		break;
	}
}


void Shurikein::OnCollision(MObject *otherObj, char* sideCollided) {
	if (phase == 1) {
		if (otherObj->tag == "static") {
			if (sideCollided == (char*)"right") {
				dirRight = 1;
			}
			if (sideCollided == (char*)"left") {
				dirRight = -1;

				phase1Count--;
			}
			if (sideCollided == (char*)"top") {
				movey = 0;
				state = STATE_PHASE1_ROLL;
			}
		}
	}

	if (phase == 2) {
		if (otherObj->tag == "static") {
			if (sideCollided == (char*)"right") {
				movex = 0;
				movey = PHASE2_SPEED * -1;
			}
			if (sideCollided == (char*)"bottom") {
				movex = PHASE2_SPEED;
				movey = 0;
			}
			if (sideCollided == (char*)"left") {
				movex = 0;
				movey = PHASE2_SPEED;
			}
			if (sideCollided == (char*)"top") {
				movex = PHASE2_SPEED * -1;
				movey = 0;

				phase2Count--;
			}
		}
	}

	if (phase == 3) {
		if (otherObj->tag == "static") {
			if (sideCollided == "right" || sideCollided == "left") {
				dirRight *= -1;
			}
			if (sideCollided == "top") {
				movey = -20;

				phase3Count--;
			}
		}
	}
}


void Shurikein::Update() {
	delta_t++;

	if (phase == 0) {
		if (state != STATE_APPEARING) {
			SetState(STATE_APPEARING);
		}

		if (delta_t >= 82) {
			delta_t = 0;
			phase = 1;
			SetState(STATE_PHASE1_ROLL);
		}
	}

	if (phase == 1) {
		if (delta_t >= 150) {
			movex = 10 * dirRight;
		}
		if (state == STATE_PHASE1_ROLL) {
			movey = 0;
		}
		if (state == STATE_PHASE1_JUMP) {
			movey += 1;
		}

		//Tạm thời, sẽ gọi khi bị đạn bắn trúng lúc đang ROLL phase1
		if (delta_t == 190) {
			movey = -20;
			SetState(STATE_PHASE1_JUMP);
		}
	}

	if (phase == 2) {
		if (delta_t == 150) {
			movex = PHASE2_SPEED * -1;
			movey = 0;
			//Sẽ xử lý tiếp lúc va chạm
		}
	}

	if (phase == 3) {
		if (delta_t == 150) {
			dirRight = -1;
			movey = -20; //Gia tri lực nhảy này sẽ random sau mỗi lần bật
			movex = 5 * dirRight;
		}
		if (delta_t > 150) {
			movey += 1;
			movex = 5 * dirRight;
		}
	}

	if (phase1Count <= 0) {
		phase1Count = 3;
		SetState(STATE_PHASE2);
		phase = 2;
		delta_t = 0;
		movex = 0;
		movey = 0;
	}
	if (phase2Count <= 0) {
		phase2Count = 1;
		SetState(STATE_PHASE3);
		phase = 3;
		delta_t = 0;
		movex = 0;
		movey = 0;
	}
	if (phase3Count <= 0) {
		phase3Count = 6;
		SetState(STATE_PHASE1_ROLL);
		phase = 1;
		delta_t = 0;
		movex = 0;
		movey = 0;
	}
}

void Shurikein::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(-1 * dirRight, 1); //-1 do hình bị ngược
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	x += movex;
	y += movey;
	anim->Animate(matrix);
}