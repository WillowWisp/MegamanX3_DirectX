#include "Megaman.h"

Megaman::Megaman()
{
	//x = (LEFTWALL_X + RIGHTWALL_X) / 2;
	x = LEFTWALL_X + 100;
	y = GROUND_Y - 50;
	isHitGround = false;

	delta_t = -1;
	movex = 0;
	movey = 0;
	dirUp = 1;
	dirRight = 1;
	jumpHold = false;
	dashHold = false;
	shootHold = false;
	inMidAir = false;
	leftHold = false;
	rightHold = false;
	wallJump = false;
	dashKick = false;
	moving = false;
	jumpAfterDash = false;
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

void Megaman::OnCollision(MObject *otherObj, float normalx, float normaly) {
	if (normalx == 0.0f && normaly == -1.0f) {
		isHitGround = true;
		this->collideObject = otherObj;
		GAMELOG("Co Va Cham");
	}

	if ((normalx == 1.0f || normalx == -1.0f) && normaly == 0.0f) {

	}
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
	case STATE_JUMPING:
		SetAnimState(34, 36, ANIM_DELAY);
		//movex = jumpAfterDash ? DASH_SPEED : MEGAMAN_SPEED;
		break;
	case STATE_FALLING:
		SetAnimState(36, 40, ANIM_DELAY);
		break;
	case STATE_WALL_SLIDING:
		SetAnimState(48, 50, ANIM_DELAY + 1);
		//jumpAfterDash = false;
		break;
	case STATE_WALL_KICKING:
		SetAnimState(51, 52, ANIM_DELAY + 1);
		//jumpAfterDash = false;
		break;
	default:
		SetAnimState(7, 10, ANIM_DELAY + 4);
		movex = 0;
		movey = 0;
		break;
	}
}

void Megaman::Upd() {
	isHitGround = false;
}

void Megaman::Update()
{
	movey += GRAVITY * 10;
	//Check if megaman is moving
	if (Input::KeyDown(DIK_LEFT)) {

		if (state != STATE_WALL_KICKING) {		// can't change velocity while wall kicking
			movex = MEGAMAN_SPEED;
			moving = true;
		}

		if (HorizontalDirChanged(-1)
			&& !(state == STATE_DASHING)		// can't change direction while dashing
			&& !(state == STATE_WALL_KICKING)
			&& !(state == STATE_WALL_SLIDING))	// can't change direction while wall sliding
		{
			ChangeDirHorizontal();
		}
	}
	else if (Input::KeyDown(DIK_RIGHT)) {

		if (state != STATE_WALL_KICKING) {
			movex = MEGAMAN_SPEED;
			moving = true;
		}

		if (HorizontalDirChanged(1)
			&& !(state == STATE_DASHING)
			&& !(state == STATE_WALL_KICKING)
			&& !(state == STATE_WALL_SLIDING))
		{
			ChangeDirHorizontal();
		}		
	}
	else {
		moving = false;
		if (movex > 0) //if not bouncing
			movex = 0;
	}

	//Check states
	//if (state == STATE_WALL_SLIDING || (inMidAir && HitWall())) {									//CANCER
	//	if (StateChanged(STATE_WALL_SLIDING))
	//		SetState(STATE_WALL_SLIDING);

	//	//if (Input::KeyDown(DIK_X)) {
	//	//	anim->ChangeAnimFrames(53, 55);
	//	//}

	//	if (HitGround()) {
	//		if (anim->curframe == anim->endframe && anim->animcount == anim->animdelay) {
	//			SetState(STATE_IDLE);
	//		}
	//		else {
	//			anim->animdelay = 0;
	//		}
	//		y = GROUND_Y;
	//		movey = 0;
	//		inMidAir = false;
	//	}
	//	//wall kick
	//	else if (Input::KeyDown(DIK_Z) && !jumpHold) {
	//		SetState(STATE_WALL_KICKING);
	//		delta_t = 0;
	//		movex = -WALL_JUMP_SPEED;
	//		movey = WALL_JUMP_SPEED;
	//	}
	//	//if not holding left/right key or holding both keys -> fall
	//	else if (((dirRight > 0 && !rightHold) || (dirRight < 0 && !leftHold))) {
	//		jumpAfterDash = false;
	//		wallJump = false;
	//		SetState(STATE_FALLING);
	//		delta_t = 0;
	//		x += 20 * (-dirRight); //bounce back
	//		//movex = 0;
	//		movey = WALL_SLIDE_SPEED;
	//		//movey = 10;
	//	}
	//	else {

	//		movey = WALL_SLIDE_SPEED;

	//		if (anim->curframe == anim->beginframe + 2) {
	//			anim->animcount = anim->animdelay;
	//		}
	//	}
	//}
	//else if (state == STATE_WALL_KICKING) {															//CANCER2
	//	//check if kick velocity is positive
	//	if (movex + delta_t * KICK_ANTI_FORCE >= 0)
	//	{
	//		SetState(STATE_JUMPING);
	//		if (Input::KeyDown(DIK_X)) {
	//			anim->ChangeAnimFrames(42, 43);
	//		}
	//		anim->curframe = anim->beginframe + 2;
	//		delta_t = WALL_JUMP_SPEED / 2;
	//		movey += delta_t * GRAVITY;	//Quick maths
	//		movex = 0;
	//		if (rightHold && HorizontalDirChanged(1)) {
	//			dirRight = 1;
	//		}
	//		else if (leftHold && HorizontalDirChanged(-1)) {
	//			dirRight = -1;
	//		}
	//	}
	//	else {
	//		if (Input::KeyDown(DIK_X)) {
	//			anim->ChangeAnimFrames(56, 57);
	//		}

	//		if (anim->curframe == anim->endframe) {
	//			anim->animcount = anim->animdelay; //Animation shits
	//		}

	//		delta_t++;
	//		if (jumpAfterDash) {
	//			movex = movex + delta_t * (KICK_ANTI_FORCE / 2); //Quick physics
	//		}
	//		else {
	//			movex = movex + delta_t * KICK_ANTI_FORCE;
	//		}
	//	}
	//}
	//else 
	if (!isHitWall) {
		if (state == STATE_JUMPING) {																//ASCENDING
			//if shooting key is pressed then change animation
			if (Input::KeyDown(DIK_X)) {
				//SetState(STATE_DASHING_N_SHOOTING);
				anim->ChangeAnimFrames(41, 43);

			}
			else if (!Input::KeyDown(DIK_X)) {
				//SetState(STATE_DASHING);
				anim->ChangeAnimFrames(34, 36);
			}

			//check if velocity is negative (falling down)
			if (!jumpHold || movey < 0) {
				SetState(STATE_FALLING);
				if (Input::KeyDown(DIK_X)) {
					anim->ChangeAnimFrames(43, 47);
				}
				delta_t = 1;
				movey = delta_t * GRAVITY;	//Quick maths
			}
			else {
				if (anim->curframe == anim->endframe) {
					anim->animcount = anim->animdelay; //Animation shits
				}

				if (jumpAfterDash && moving) {
					movex = DASH_SPEED;
				}

				delta_t++;
				movey = movey + delta_t * GRAVITY;	//Quick physics
			}

		}
		else if (state == STATE_FALLING) {															//FALLING
			//if shooting key is pressed then change animation
			if (Input::KeyDown(DIK_X)) {
				//SetState(STATE_DASHING_N_SHOOTING);
				anim->ChangeAnimFrames(43, 47);

			}
			else if (!Input::KeyDown(DIK_X)) {
				//SetState(STATE_DASHING);
				anim->ChangeAnimFrames(36, 40);
			}

			//(Temp, will use collision for this shit) Check if megaman hit the ground
			if (isHitGround) {
				if (anim->curframe == anim->endframe && anim->animcount == anim->animdelay) {
					SetState(STATE_IDLE);
				}
				else {
					anim->animdelay = 0;
				}
				y = this->collideObject->y - this->collideObject->height / 2 - this->height / 2;
				movey = 0;
				inMidAir = false;
			}
			else {
				if (anim->curframe == anim->beginframe + 2)
					anim->animcount = anim->animdelay;
				if (jumpAfterDash && moving)
					movex = DASH_SPEED;
				if (jumpAfterDash && wallJump) {
					movex = WALL_DASH_BOUNCE_SPEED;
				}

				delta_t++;
				movey = movey + delta_t * GRAVITY;
			}
		}
		else if (Input::KeyDown(DIK_Z) && !jumpHold) {												//START JUMPING
			if (state == STATE_DASHING)
				jumpAfterDash = true;
			if (StateChanged(STATE_JUMPING))
				SetState(STATE_JUMPING);

			delta_t = 0;
			movey = JUMP_SPEED + delta_t * GRAVITY;
			inMidAir = true;
		}
		else if (state == STATE_DASHING) {															//DASHING
			//if shooting key is pressed then change animation
			if (Input::KeyDown(DIK_X)) {
				//SetState(STATE_DASHING_N_SHOOTING);
				anim->ChangeAnimFrames(62, 63);

			}
			else if (!Input::KeyDown(DIK_X)) {
				//SetState(STATE_DASHING);
				anim->ChangeAnimFrames(60, 61);
			}

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
			//continue dashing
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
		else if (Input::KeyDown(DIK_V) && !dashHold) {												//START DASHING

			if (StateChanged(STATE_DASHING))
				SetState(STATE_DASHING);

			delta_t = 0;
			movex = DASH_SPEED;
		}
		else if (Input::KeyDown(DIK_X)) {															//SHOOTING
			if (StateChanged(STATE_SHOOTING))
				SetState(STATE_SHOOTING);
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
				//movey = 0;
				//jumpAfterDash = true;
			}
			jumpAfterDash = false;
			wallJump = false;
		}
	}
	else {
		if (StateChanged(STATE_IDLE))
			SetState(STATE_IDLE);
		movex = 0;
		movey = 0;
		jumpAfterDash = false;
		wallJump = false;
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

	if (Input::KeyDown(DIK_LEFT)) {
		leftHold = true;
	}
	else {
		leftHold = false;
	}

	if (Input::KeyDown(DIK_RIGHT)) {
		rightHold = true;
	}
	else {
		rightHold = false;
	}

	/*if (HitWall()) {
		if (x - abs(movex) > LEFTWALL_X)
			x = RIGHTWALL_X - movex;
		else
			x = LEFTWALL_X + movex;
	}*/

	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex * dirRight, y - movey);
	D3DXVECTOR2 scale = D3DXVECTOR2(2 * dirRight, 2);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &translation);
	MObject::Update();
}
