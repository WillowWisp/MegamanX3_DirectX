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
	jumpHold = false;
	dashHold = false;
	moving = false;
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
		//movex = MEGAMAN_SPEED;
		break;
	case STATE_SHOOTING:
		SetAnimState(11, 12, ANIM_DELAY);
		movex = 0;
		movey = 0;
		break;
	case STATE_RUNNING_N_SHOOTING:
		SetAnimState(24, 33, ANIM_DELAY);
		//movex = MEGAMAN_SPEED;
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
	case STATE_JUMPING_N_SHOOTING:
		SetAnimState(41, 47, ANIM_DELAY);
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
	//Check if megaman is moving
	if (Input::KeyDown(DIK_LEFT)) {
		
		movex = (movex == DASH_SPEED) ? DASH_SPEED : MEGAMAN_SPEED; //conserve dash velocity for dash and jump
		moving = true;

		if (HorizontalDirChanged(-1) 
			&& !(state == STATE_DASHING || state == STATE_DASHING_N_SHOOTING)) // can't change direction while dashing
			ChangeDirHorizontal();
	}
	else if (Input::KeyDown(DIK_RIGHT)) {
		
		movex = (movex == DASH_SPEED) ? DASH_SPEED : MEGAMAN_SPEED;
		moving = true;

		if (HorizontalDirChanged(1)
			&& !(state == STATE_DASHING || state == STATE_DASHING_N_SHOOTING))
			ChangeDirHorizontal();
	}
	else {
		moving = false;
		movex = 0;
	}

	//Check states

	if (state == STATE_JUMPING) {																//ASCENDING
		//check if velocity is negative (falling down)
		if (!jumpHold || movey < 0) {
			SetState(STATE_FALLING);
			delta_t = 1;
			movey = delta_t * GRAVITY;	//Quick maths
		}

		delta_t++;
		movey = movey + delta_t * GRAVITY;	//Quick physics
		if (anim->curframe == anim->endframe) {
			anim->animcount = anim->animdelay; //Animation shits
		}
			
	}
	else if (state == STATE_FALLING) {															//FALLING
		//(Temp, will use collision for this shit) Check if megaman hit the ground
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
	else if (Input::KeyDown(DIK_Z) && !jumpHold) {												//START JUMPING

		if (StateChanged(STATE_JUMPING))
			SetState(STATE_JUMPING);

		delta_t = 0;
		movey = JUMP_SPEED + delta_t * GRAVITY;
	}
	else if (state == STATE_DASHING) {															//DASHING
		if (!dashHold) {
			delta_t = -1;
		}
		//stop if passing dash duration or not holding key
		if (delta_t == DASH_DURATION || delta_t < 0) {
			if (anim->curframe == anim->endframe && anim->animcount == anim->animdelay) {
				SetState(STATE_IDLE);
			}
			else {
				anim->curframe = anim->endframe;
			}
			movex = 0;
		}
		//if shooting key is pressed then change state 
		else if (Input::KeyDown(DIK_X)) {
			SetState(STATE_DASHING_N_SHOOTING);
		}
		//continue dashing
		else {
			delta_t++;
			anim->animcount = 0;
			movex = DASH_SPEED;
		}
	}
	else if (state == STATE_DASHING_N_SHOOTING) {												//DASHING AND SHOOTING
		if (!dashHold) {
			delta_t = -1;
		}
		if (delta_t == DASH_DURATION || delta_t < 0) {
			if (anim->curframe == anim->endframe && anim->animcount == anim->animdelay) {
				SetState(STATE_IDLE);
			}
			else {
				anim->curframe = anim->endframe;
			}
			movex = 0;
		}
		//if shooting key is released then change state
		else if (!Input::KeyDown(DIK_X)) {
			SetState(STATE_DASHING);
		}
		else {
			delta_t++;
			anim->animcount = 0;
			movex = DASH_SPEED;
		}
	}
	else if (Input::KeyDown(DIK_X) && moving) {													//SHOOTING AND RUNNING

		if (StateChanged(STATE_RUNNING_N_SHOOTING))
			SetState(STATE_RUNNING_N_SHOOTING);

	}
	else if (Input::KeyDown(DIK_X)) {															//SHOOTING
		if (StateChanged(STATE_SHOOTING))
			SetState(STATE_SHOOTING);
	}
	else if (Input::KeyDown(DIK_V) && !dashHold) {												//START DASHING

		if (StateChanged(STATE_DASHING))
				SetState(STATE_DASHING);

			delta_t = 0;
			movex = DASH_SPEED;			
	}
	else {
		//Only running key is pressed
		if (moving) {																			//RUNNING
			if (StateChanged(STATE_RUNNING))
				SetState(STATE_RUNNING);

		}
		//No key is pressed
		else {																					//IDLE
			if (StateChanged(STATE_IDLE))
				SetState(STATE_IDLE);
			movex = 0;
			movey = 0;

		}
	}

	if (Input::KeyDown(DIK_Z)) {
		jumpHold = true;
	}
	else {
		jumpHold = false;
	}

	if (Input::KeyDown(DIK_V)) {
		dashHold = true;
	}
	else {
		dashHold = false;
	}

	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex * dirRight, y - movey);
	D3DXVECTOR2 scale = D3DXVECTOR2(2 * dirRight, 2);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &translation);
	MObject::Update();
}
