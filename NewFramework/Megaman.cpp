#include "Megaman.h"

Megaman::Megaman()
{
	//x = (LEFTWALL_X + RIGHTWALL_X) / 2;
	/*x = LEFTWALL_X + 100;*/
	x = 70;
	y = 500;
	x = 200;
	y = 800;
	//x = 0;
	//y = 0;
	isHitGround = false;
	isHitWallRight = false;
	isHitWallLeft = false;
	curGroundY = 1000000;
	curCeilY = -1000000;
	curLeftWallX = -1000000;
	curRightWallX = 1000000;

	//width = 30;
	//height = 30;

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
	//SetState(STATE_IDLE);
	SetState(STATE_FALLING);
	SetWidthHeight();
}


Megaman::~Megaman()
{
}

void Megaman::OnCollision(MObject *otherObj, char* sideCollided) {
	collideObject = otherObj;
	//movey = 0;
	//movex = 0;
	//isHitGround = true;
	//GAMELOG("Collided %d", ((x + width) - collideObject->x));
	//GAMELOG(sideCollided);
	//if (sideCollided == (char*)"top" || sideCollided == (char*)"unknown") {
	//	isHitGround = true;
	//}

	if (sideCollided == (char*)"left") {
		isHitWallLeft = true;
		isHitWallRight = false;
		
		/*GAMELOG(sideCollided);
		GAMELOG("Collided %d", this->collideObject->x - this->collideObject->width / 2 - width / 2 - 2 - movex);*/
		/*GAMELOG("Collided %d", this->collideObject->x - this->collideObject->width / 2);*/
	}

	if (sideCollided == (char*)"right") {
		isHitWallRight = true;
		isHitWallLeft = false;
		//GAMELOG(sideCollided);
		////GAMELOG("Collided %d", this->collideObject->x + this->collideObject->width / 2 + width / 2 + 5);
		//GAMELOG("Collided %d", this->collideObject->x + this->collideObject->width / 2);
	}

	//if (sideCollided == (char*)"unknown") {
	//	if (state == STATE_FALLING) {
	//		isHitGround = true;
	//	}
	//	else if (movex > 0 && dirRight == 1) {
	//		isHitWallLeft;
	//	}
	//	else if (movex > 0 && dirRight == -1) {
	//		isHitWallRight;
	//	}
	//}

}

void Megaman::SetCamera(Camera * newCamera)
{
	this->camera = newCamera;
}

void Megaman::SetState(int newState)
{
	state = newState;
	switch (newState)
	{
	case STATE_IDLE:
		SetAnimState(7, 9, ANIM_DELAY + 10);
		SetWidthHeight();
		movex = 0;
		movey = 0;
		isHitGround = true;
		//GAMELOG("idle");
		break;
	case STATE_RUNNING:
		SetAnimState(13, 23, ANIM_DELAY - 1);
		SetWidthHeight();
		//movex = MEGAMAN_SPEED;
		break;
	case STATE_SHOOTING:
		SetAnimState(11, 12, ANIM_DELAY);
		SetWidthHeight();
		movex = 0;
		movey = 0;
		break;
	case STATE_RUNNING_N_SHOOTING:
		SetAnimState(24, 33, ANIM_DELAY);
		SetWidthHeight();
		//movex = MEGAMAN_SPEED;
		break;
	case STATE_DASHING:
		SetAnimState(60, 61, ANIM_DELAY + 5);
		SetWidthHeight();
		break;
	case STATE_JUMPING:
		SetAnimState(34, 36, ANIM_DELAY);
		SetWidthHeight();
		GAMELOG("jumping");
		//movex = jumpAfterDash ? DASH_SPEED : MEGAMAN_SPEED;
		break;
	case STATE_FALLING:
		SetAnimState(36, 40, ANIM_DELAY);
		SetWidthHeight();
		delta_t = 0;
		inMidAir = true;
		//GAMELOG("falling");
		break;
	case STATE_WALL_SLIDING:
		SetAnimState(48, 50, ANIM_DELAY);
		GAMELOG("sliding");
		//jumpAfterDash = false;
		break;
	case STATE_WALL_KICKING:
		SetAnimState(51, 52, ANIM_DELAY + 1);
		SetWidthHeight();
		//jumpAfterDash = false;
		break;
	default:
		SetAnimState(7, 10, ANIM_DELAY + 4);
		SetWidthHeight();
		movex = 0;
		movey = 0;
		break;
	}
}

bool Megaman::HitGround() {
	return (y + height / 2 >= curGroundY);
}

bool Megaman::HitCeil() {
	return (y - height / 2 <= curCeilY);
}

bool Megaman::HitWall() {
	return (x - width / 2 <= curLeftWallX || x + width / 2 >= curRightWallX);
}

bool Megaman::CloseToWall() {
	return (x - width / 2 <= curLeftWallX - 2 * dirRight || x + width / 2 >= curRightWallX - 2 * dirRight);
}

void Megaman::Update()
{
	if (Input::KeyDown(DIK_S)) {
		movey = 3;
	}
	else if (Input::KeyDown(DIK_W)) {
		movey = -3;
	}
	if (Input::KeyDown(DIK_D)) {
		movex = 3;
	}
	else if (Input::KeyDown(DIK_A)) {
		movex = -3;
	}
	
	//Check if megaman is moving
	if (Input::KeyDown(DIK_LEFT)) {
		//camera->position.x -= (movex-5);
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
		//camera->position.x += (movex )-5;
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


	if (!HitGround() 
		&& state != STATE_JUMPING 
		&& state != STATE_WALL_SLIDING
		&& state != STATE_WALL_KICKING) {

		if (StateChanged(STATE_FALLING))
			SetState(STATE_FALLING);
		//delta_t = 0;
	}
	else {
		//if (StateChanged(STATE_IDLE))
		//	SetState(STATE_IDLE);
	}


	//Check states
	if (state == STATE_WALL_SLIDING || (inMidAir && HitWall())) {									//CANCER
		if (StateChanged(STATE_WALL_SLIDING))
			SetState(STATE_WALL_SLIDING);

		//if (Input::KeyDown(DIK_X)) {
		//	anim->ChangeAnimFrames(53, 55);
		//}

		if (HitGround()) {
			if (anim->curframe == anim->endframe && anim->animcount == anim->animdelay) {
				SetState(STATE_IDLE);
			}
			else {
				anim->animdelay = 0;
			}
			y = curGroundY - (height / 2) + 2;
			movey = 0;
			inMidAir = false;
		}
		//wall kick
		else if (Input::KeyDown(DIK_Z) && !jumpHold) {
			SetState(STATE_WALL_KICKING);
			delta_t = 0;
			movex = -WALL_JUMP_SPEED;
			movey = -WALL_JUMP_SPEED;
		}
		//if not holding left/right key or holding both keys -> fall
		else if (((dirRight > 0 && !rightHold) || (dirRight < 0 && !leftHold)) || !HitWall()) {
			jumpAfterDash = false;
			wallJump = false;
			SetState(STATE_FALLING);
			delta_t = -1;
			x += 20 * (-dirRight); //bounce back
			//movex = 0;
			//movey = -WALL_SLIDE_SPEED;
			movey = 0;
			//movey = 10;
		}
		else {

			movey = -WALL_SLIDE_SPEED;

			if (anim->curframe == anim->endframe) {
				anim->animcount = anim->animdelay;
			}
		}
	}
	else if (state == STATE_WALL_KICKING) {															//CANCER2
		//check if kick velocity is positive
		if (movex + delta_t * KICK_ANTI_FORCE >= 0)
		{
			SetState(STATE_JUMPING);
			if (Input::KeyDown(DIK_X)) {
				anim->ChangeAnimFrames(42, 43);
			}
			anim->curframe = anim->beginframe + 2;
			delta_t = WALL_JUMP_SPEED / 2;
			/*delta_t = 0;*/
			movey -= delta_t * GRAVITY;	//Quick maths
			movex = 0;
			if (rightHold && HorizontalDirChanged(1)) {
				dirRight = 1;
			}
			else if (leftHold && HorizontalDirChanged(-1)) {
				dirRight = -1;
			}
		}
		else {
			if (Input::KeyDown(DIK_X)) {
				anim->ChangeAnimFrames(56, 57);
			}

			if (anim->curframe == anim->endframe) {
				anim->animcount = anim->animdelay; //Animation shits
			}

			delta_t++;
			if (jumpAfterDash) {
				movex = movex + delta_t * (KICK_ANTI_FORCE / 2); //Quick physics
			}
			else {
				movex = movex + delta_t * KICK_ANTI_FORCE;
			}
		}
	}
	else if (state == STATE_DASHING) {															//DASHING

		if (Input::KeyDown(DIK_Z) && !jumpHold) {

			if (StateChanged(STATE_JUMPING))
				SetState(STATE_JUMPING);

			jumpAfterDash = true;
			delta_t = 0;
			movey = -(JUMP_SPEED + delta_t * GRAVITY);
			inMidAir = true;
		}

		else {

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
	}
	else 
	if (!(HitWall())) {
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
			if (!jumpHold || movey > 0) {
				SetState(STATE_FALLING);
				if (Input::KeyDown(DIK_X)) {
					anim->ChangeAnimFrames(43, 47);
				}
				delta_t = 1;
				movey = -(delta_t * GRAVITY);	//Quick maths
			}
			else {
				if (anim->curframe == anim->endframe) {
					anim->animcount = anim->animdelay; //Animation shits
				}

				if (jumpAfterDash && moving) {
					movex = DASH_SPEED;
				}

				delta_t++;
				movey -= delta_t * GRAVITY;	//Quick physics
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
			//if (isHitGround || anim->curframe > anim->beginframe + 2) {
			if ((height / 2) + y >= curGroundY) {
				if (anim->curframe == anim->endframe && anim->animcount == anim->animdelay) {
					SetState(STATE_IDLE);
				}
				else {
					anim->animdelay = 0;
				}
				//SetState(STATE_IDLE);
				//y = this->collideObject->y - this->collideObject->height / 2 - height / 2 + 5 + 10;
				//y = this->collideObject->y - this->collideObject->height / 2 + 5;
				//x = 0;
				//y = GROUND_Y;
				y = curGroundY - (height / 2) + 2;
				movey = 0;
				inMidAir = false;
				delta_t = 0;
				isHitGround = true;
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
				movey -= delta_t * GRAVITY;
			}
		}
		else if (Input::KeyDown(DIK_Z) && !jumpHold) {												//START JUMPING
			if (state == STATE_DASHING)
				jumpAfterDash = true;
			if (StateChanged(STATE_JUMPING))
				SetState(STATE_JUMPING);

			delta_t = 0;
			movey = -(JUMP_SPEED + delta_t * GRAVITY);
			//y -= 55;
			//movey = -100;
			inMidAir = true;
			//isHitGround = false;
		}
		else if (Input::KeyDown(DIK_X) && moving && !CloseToWall()) {													//SHOOTING AND RUNNING

			if (StateChanged(STATE_RUNNING_N_SHOOTING))
				SetState(STATE_RUNNING_N_SHOOTING);

		}
		else if (Input::KeyDown(DIK_V) && !dashHold && !CloseToWall()) {												//START DASHING

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
			if (moving && !CloseToWall()) {																			//RUNNING
				if (StateChanged(STATE_RUNNING))
					SetState(STATE_RUNNING);

			}
			//No key is pressed
			else {																					//IDLE
				if (StateChanged(STATE_IDLE))
					SetState(STATE_IDLE);
				movex = 0;
				//movey = 0;
				//isHitGround = true;
				//jumpAfterDash = true;
			}
			jumpAfterDash = false;
			wallJump = false;
		}
	}
	else {
		//if (moving && ((isHitWallLeft && dirRight == -1) || (isHitWallRight && dirRight == 1))) {
		//	if (StateChanged(STATE_RUNNING))
		//		SetState(STATE_RUNNING);
		//}
		//else {
			if (StateChanged(STATE_IDLE))
				SetState(STATE_IDLE);
			movex = 0;
			movey = 0;
			jumpAfterDash = false;
			wallJump = false;
			//isHitGround = true;
		//}

		//if (StateChanged(STATE_RUNNING))
		//	SetState(STATE_RUNNING);
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

	//if (isHitWallLeft) {
	//	//movex = 5;
	//	//ChangeDirHorizontal();
	//	//x = this->collideObject->y - this->collideObject->height / 2 - height / 2 + 5;
	//	//x -= 20;
	//	//x += ((x + width) - collideObject->x);
	//	GAMELOG("collided left: %d", x);
	//	x = this->collideObject->x - this->collideObject->width / 2 - width / 2 - 0 - movex;
	//	
	//	GAMELOG("%d", x);
	//	isHitWallLeft = false;
	//	//movex = 0;

	//	//if (StateChanged(STATE_IDLE))
	//	//	SetState(STATE_IDLE);
	//	//if (StateChanged(STATE_RUNNING))
	//	//	SetState(STATE_RUNNING);
	//}
	//if (isHitWallRight) {
	//	//movex = 5;
	//	//ChangeDirHorizontal();
	//	//x += 20;
	//	//x -= ((x + width) - collideObject->x);
	//	GAMELOG("collided right: %d", x);
	//	x = this->collideObject->x + this->collideObject->width / 2 + width / 2 + 0 + movex;
	//	GAMELOG("%d", x);
	//	isHitWallRight = false;
	//	//movex = 0;

	//	//if (StateChanged(STATE_IDLE))
	//	//	SetState(STATE_IDLE);
	//	//if (StateChanged(STATE_RUNNING))
	//	//	SetState(STATE_RUNNING);
	//}

	//if (x + movex * dirRight <= curLeftWallX || x + movex * dirRight >= curRightWallX) {
	//	if (x - abs(movex) > curLeftWallX)
	//		x = curRightWallX - movex;
	//	else
	//		x = curLeftWallX + movex;
	//}
	if (HitWall()) {
		/*if (x + width / 2 >= curRightWallX)
			x = curRightWallX - width / 2;
		else if (x - width / 2 <= curLeftWallX)
			x = curLeftWallX + width / 2;*/
		if (x - width / 2 <= curLeftWallX)
			x = curLeftWallX + width / 2 + 2;
		else if (x + width / 2 >= curRightWallX)
			x = curRightWallX - width / 2 - 2;
	}

	if (HitCeil()) {
		y = curCeilY + height / 2 + 2;
	}

	//if (isHitWallRight || isHitWallLeft) {
	//	if (isHitWallLeft)
	//		x = RIGHTWALL_X - 5;
	//	else
	//		x = LEFTWALL_X + 5;
	//}

	if (Input::KeyDown(DIK_F)) {
		x = 0;
		y = 0;
		x = 200;
		y = 400;
		movex = 0;
		movey = 0;
		delta_t = 0;
	}

	if (Input::KeyDown(DIK_Y)) {
		y = 0;
		movex = 0;
		movey = 0;
		delta_t = 0;
	}

	if (Input::KeyDown(DIK_L)) {
		GAMELOG("left %d", curLeftWallX);
		GAMELOG("right %d", curRightWallX);
		GAMELOG("ground %d", curGroundY);
		GAMELOG("ceil %d", curCeilY);
	}

	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 translate = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + translate;

	D3DXVECTOR2 scale = D3DXVECTOR2(2 * dirRight, 2);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	MObject::Update();

	//isHitGround = false;
}

//void Megaman::SetWidthHeight() {
//	//width = 34;
//	//height = 30;
//}

void Megaman::SetSignedMoveX() {
	if (dirRight == -1) {
		movex *= -1;
	}
	//if (state == STATE_WALL_KICKING) {
	//	movex *= -1;
	//}
}

void Megaman::SetUnsignedMoveX() {
	movex = abs(movex);
	if (state == STATE_WALL_KICKING) {
		movex *= -1;
	}
}