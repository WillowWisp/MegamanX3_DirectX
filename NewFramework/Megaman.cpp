#include "Megaman.h"

Megaman::Megaman()
{
	x = 400;
	y = 380;
	/*width = 20;
	height = 25;*/
	delta_t = -1;
	//prevKey = NULL;
	movex = 0;
	movey = 0;
	dirUp = 1;
	dirRight = 1;
	anim = new Animation(64, 7, 9, ANIM_DELAY + 10);

	char s[50];
	for (int i = 0; i < 64; i++) {
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
		movex = 0;
		movey = 0;
		break;
	case STATE_RUNNING:
		SetAnimState(13, 23, ANIM_DELAY - 1);
		movex = MEGAMAN_SPEED;
		break;
	case STATE_SHOOTING:
		SetAnimState(11, 12, ANIM_DELAY);
		movex = 0;
		movey = 0;
		break;
	case STATE_RUNNING_N_SHOOTING:
		SetAnimState(24, 33, ANIM_DELAY);
		movex = MEGAMAN_SPEED;
		break;
	case STATE_DASHING:
		SetAnimState(60, 61, ANIM_DELAY + 5);
		break;
	case STATE_DASHING_N_SHOOTING:
		SetAnimState(62, 63, ANIM_DELAY + 5);
		break;
	case STATE_JUMPING:
		SetAnimState(34, 36, ANIM_DELAY);
		break;
	case STATE_FALLING:
		SetAnimState(36, 40, ANIM_DELAY);
		break;
	default:
		SetAnimState(7, 10, ANIM_DELAY + 4);
		movex = 0;
		movey = 0;
		break;
	}
}

void Megaman::Update()
{
	if (state == STATE_JUMPING) {
		if (!Input::KeyDown(DIK_Z) || movey < 0) {
			SetState(STATE_FALLING);
			delta_t = 1;
			movey = delta_t * GRAVITY;
		}

		delta_t++;
		movey = movey + delta_t * GRAVITY;
		if (anim->curframe == anim->endframe) {
			anim->animcount = anim->animdelay;
		}
			
	}
	else if (state == STATE_FALLING) {
		if (y >= (GROUND_Y - anim->sprite[anim->curframe]->height / 2)) {
			if (anim->curframe == anim->endframe && anim->animcount == anim->animdelay) {
				SetState(STATE_IDLE);
			}
			else {
				anim->animdelay = 0;
			}
			y = GROUND_Y;
			movey = 0;
		}
		else {
			delta_t++;
			movey = movey + delta_t * GRAVITY;

			if (anim->curframe == 38)
				anim->animcount = anim->animdelay;
		}
	}
	else if (state == STATE_DASHING) {
		if (!Input::KeyDown(DIK_V)) {
			delta_t = -1;
		}

		if (delta_t == DASH_DURATION || delta_t < 0) {
			if (anim->curframe == anim->endframe && anim->animcount == anim->animdelay) {
				SetState(STATE_IDLE);
			}
			else {
				anim->curframe = anim->endframe;
				//anim->animcount = 0;
			}
			movex = 0;
		}
		else if (Input::KeyDown(DIK_X)) {
			SetState(STATE_DASHING_N_SHOOTING);
		}
		else {
			delta_t++;
			anim->animcount = 0;
		}
	}
	else if (state == STATE_DASHING_N_SHOOTING) {
		if (!Input::KeyDown(DIK_V)) {
			delta_t = -1;
		}
		if (delta_t == DASH_DURATION || delta_t < 0) {
			if (anim->curframe == anim->endframe && anim->animcount == anim->animdelay) {
				SetState(STATE_IDLE);
			}
			else {
				anim->curframe = anim->endframe;
				//anim->animcount = 0;
			}
			movex = 0;
		}
		else if (!Input::KeyDown(DIK_X)) {
			SetState(STATE_DASHING);
		}
		else {
			delta_t++;
			anim->animcount = 0;
		}
	}
	else if (Input::KeyDown(DIK_X) && !(Input::KeyDown(DIK_LEFT) || Input::KeyDown(DIK_RIGHT))) {
		if (StateChanged(STATE_SHOOTING))
			SetState(STATE_SHOOTING);
	}
	else if (Input::KeyDown(DIK_X) && (Input::KeyDown(DIK_LEFT) || Input::KeyDown(DIK_RIGHT))) {

		if (StateChanged(STATE_RUNNING_N_SHOOTING))
			SetState(STATE_RUNNING_N_SHOOTING);

		movex = MEGAMAN_SPEED;

		if (Input::KeyDown(DIK_LEFT)) {
			if (HorizontalDirChanged(-1))
				ChangeDirHorizontal();
		}
		else if (Input::KeyDown(DIK_RIGHT)) {
			if (HorizontalDirChanged(1))
				ChangeDirHorizontal();
		}
	}
	else if (Input::KeyDown(DIK_LEFT)) {
		if (StateChanged(STATE_RUNNING))
			SetState(STATE_RUNNING);

		movex = MEGAMAN_SPEED;

		if (HorizontalDirChanged(-1))
			ChangeDirHorizontal();
	}
	else if (Input::KeyDown(DIK_RIGHT)) {
		if (StateChanged(STATE_RUNNING))
			SetState(STATE_RUNNING);

		movex = MEGAMAN_SPEED;

		if (HorizontalDirChanged(1))
			ChangeDirHorizontal();
	}
	else if (Input::KeyDown(DIK_Z) && state != STATE_JUMPING && state != STATE_FALLING) {

		//is key not being held down?
		if (delta_t < 0) {
			if (StateChanged(STATE_JUMPING))
				SetState(STATE_JUMPING);

			delta_t = 0;
			movey = JUMP_SPEED + delta_t * GRAVITY;

			if (Input::KeyDown(DIK_LEFT)) {
				movex = MEGAMAN_SPEED;

				if (HorizontalDirChanged(-1))
					ChangeDirHorizontal();
			}
			else if (Input::KeyDown(DIK_RIGHT)) {
				movex = MEGAMAN_SPEED;

				if (HorizontalDirChanged(1))
					ChangeDirHorizontal();
			}
		}
	}
	else if (Input::KeyDown(DIK_V)) {

		//is key not being held down?
		if (delta_t < 0) {
			if (StateChanged(STATE_DASHING))
				SetState(STATE_DASHING);

			delta_t = 0;
			movex = DASH_SPEED;			
		}

	}
	else {
		if (StateChanged(STATE_IDLE))
			SetState(STATE_IDLE);
		movex = 0;
		movey = 0;
		//prevKey = NULL;
		delta_t = -1;
	}

	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex * dirRight, y - movey);
	D3DXVECTOR2 scale = D3DXVECTOR2(2 * dirRight, 2);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &translation);
	MObject::Update();
}
