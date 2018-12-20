#include "Megaman.h"

Megaman::Megaman()
{
	x = 133;
	y = 1800;
	//x = 1400;
	//y = 1800;
	//x = 2956;
	//y = 850;
	//x = 12811;
	//y = 3900;
	tag = (char*)"megaman";
	curGroundY = 1000000;
	curCeilY = -1000000;
	curLeftWallX = -1000000;
	curRightWallX = 1000000;

	delta_t = -1;
	energy_t = -1;
	invulnerable_t = -1;
	forcedAnim_t = -1;
	shootingAnimDelay = -1;
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
	shooting = false;
	jumpAfterDash = false;
	isVulnerable = true;
	isControllable = true;
	isHealing = false;

	HP = MEGAMAN_MAX_HP;


	//color = D3DCOLOR_ARGB(255, 150, 150, 255);

	anim = new Animation(75, 7, 9, ANIM_DELAY + 10);

	char s[50];
	for (int i = 0; i < 75; i++) {
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
	if (isVulnerable) {
		if (collideObject->tag == (char*)"enemy") {
			SetState(STATE_TAKING_DAMAGE);
			//GAMELOG("HP: %d", HP);
		}
	}
	//if (collideObject->tag == (char*)"item") {
	//	y -= 100;
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
		//SetWidthHeight();
		movex = 0;
		movey = 0;
		//GAMELOG("idle");
		break;
	case STATE_RUNNING:
		SetAnimState(13, 23, ANIM_DELAY - 1);
		//SetWidthHeight();
		//movex = MEGAMAN_SPEED;
		break;
	case STATE_SHOOTING:
		SetAnimState(11, 12, ANIM_DELAY + 2);
		//SetWidthHeight();
		movex = 0;
		movey = 0;
		break;
	case STATE_RUNNING_N_SHOOTING:
		SetAnimState(24, 33, ANIM_DELAY - 1);
		//SetWidthHeight();
		//movex = MEGAMAN_SPEED;
		break;
	case STATE_DASHING:
		SetAnimState(60, 61, ANIM_DELAY + 4);
		//SetWidthHeight();
		break;
	case STATE_JUMPING:
		SetAnimState(34, 36, ANIM_DELAY);
		//SetWidthHeight();
		//GAMELOG("jumping");
		//movex = jumpAfterDash ? DASH_SPEED : MEGAMAN_SPEED;
		break;
	case STATE_FALLING:
		SetAnimState(36, 40, ANIM_DELAY);
		//SetWidthHeight();
		delta_t = 0;
		inMidAir = true;
		//GAMELOG("falling");
		break;
	case STATE_WALL_SLIDING:
		SetAnimState(48, 50, ANIM_DELAY);
		GAMELOG("sliding");
		delta_t = 0;
		//jumpAfterDash = false;
		break;
	case STATE_WALL_KICKING:
		SetAnimState(51, 52, ANIM_DELAY + 1);
		//SetWidthHeight();
		//jumpAfterDash = false;
		break;
	case STATE_TAKING_DAMAGE:
		SetAnimState(64, 74, ANIM_DELAY);
		isControllable = false;
		isVulnerable = false;
		forcedAnim_t = 0;
		movex = -3;
		movey = -7;
		//HP -= 2;
		break;
	default:
		SetAnimState(7, 10, ANIM_DELAY);
		//SetWidthHeight();
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

void Megaman::ForcedAnimation() {
	if (state == STATE_TAKING_DAMAGE) {
		if (forcedAnim_t > TAKING_DMG_ANIMATION_TIME) {
			isControllable = true;
			//SetState(STATE_IDLE);
			return;
		}
		else {
			if (anim->beginframe == 64 && anim->curframe >= 66 && anim->curframe <= 73) {
				anim->beginframe = 66;
				anim->animdelay = ANIM_DELAY - 2;
			}
			else if (anim->curframe == 74) {
				anim->beginframe = 74;
				anim->animdelay = ANIM_DELAY - 1;
			}
			movex = -3;
			movey += 1;
			if (y + movey - 1 + height / 2 > curGroundY)
				movey = 0;
			if (HitWall()) {
				if (x - width / 2 <= curLeftWallX)
					x = curLeftWallX + width / 2 + 2;
				else if (x + width / 2 >= curRightWallX)
					x = curRightWallX - width / 2 - 2;
			}
		}
	}
	else if (isHealing) {
		//if (forcedAnim_t > HEALING_TIME) {
		if (!UI::isIncreasingHP) {
			isControllable = true;
			forcedAnim_t = -1;
			isHealing = false;
		}
		else {
			anim->animcount--;
			UI::ChangeHP(HP);
		}
	}
	forcedAnim_t++;
}

void Megaman::Heal(int healAmount) {
	if (HP >= MEGAMAN_MAX_HP)
		return;
	HP += healAmount;
	if (HP > MEGAMAN_MAX_HP)
		HP = MEGAMAN_MAX_HP;
	isControllable = false;
	isHealing = true;
	forcedAnim_t = 0;
	movex = 0;
	movey = 0;
	UI::isIncreasingHP = true;
	UI::ChangeHP(HP);
	UI::HPBar->delta_t = 0;
}

void Megaman::TakeDmg(int damage) {
	if (!isVulnerable || isHealing)
		return;
	HP -= damage;
	UI::ChangeHP(HP);
	if (HP <= 0) {
		//RIP
		HP = 0;
	}
	SetState(STATE_TAKING_DAMAGE);
}

void Megaman::Update()
{
	//if (Input::KeyDown(DIK_S)) {
	//	movey = 3;
	//}
	//else if (Input::KeyDown(DIK_W)) {
	//	movey = -3;
	//}
	//if (Input::KeyDown(DIK_D)) {
	//	movex = 3;
	//}
	//else if (Input::KeyDown(DIK_A)) {
	//	movex = -3;
	//}

	if (!isControllable) {
		ForcedAnimation();
		if (!isControllable) {
			return;
		}
	}

	//Check if megaman start shooting
	if (!shootHold && Input::KeyDown(DIK_X)) {			//Start shooting - Shoot once
		shooting = true;
		shootingAnimDelay = 0;
		energy_t++;
		int temp_dirRight = 1;
		if (state == STATE_WALL_SLIDING)
			temp_dirRight = -1;
		//Create Bullet here
		BulletsManager::CreateBullet(new MegamanBullet(x + (dirRight * temp_dirRight * width / 2), 
														y - (height / 9), dirRight * temp_dirRight, 
														0));
	}
	else if (shootHold && !Input::KeyDown(DIK_X)) {		//Shooting with energy
		shooting = true;
		shootingAnimDelay = 0;
		int temp_dirRight = 1;
		if (state == STATE_WALL_SLIDING)
			temp_dirRight = -1;
		//Create Bullet here
		if (energy_t >= CHARGED_SHOT_LV1_TIME && energy_t < CHARGED_SHOT_LV2_TIME) {
			BulletsManager::CreateBullet(new MegamanBullet(x + (dirRight * temp_dirRight * width / 2), 
															y - (height / 9), dirRight * temp_dirRight, 
															1));
		}
		else if (energy_t >= CHARGED_SHOT_LV2_TIME) {
			BulletsManager::CreateBullet(new MegamanBullet(x + (dirRight * temp_dirRight * width / 2), 
															y - (height / 9), dirRight * temp_dirRight, 
															2));
		}

		energy_t = -1;
		//color = D3DCOLOR_ARGB(255, 255, 255, 255);
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
	if (state == STATE_WALL_SLIDING 
		|| (inMidAir && HitWall() && curGroundY > y + height / 2 + HIT_GROUND_MARGIN)) {					//CANCER
		if (StateChanged(STATE_WALL_SLIDING))
			SetState(STATE_WALL_SLIDING);

		if (HitGround()) {
			if (anim->curframe == anim->endframe && anim->animcount == anim->animdelay) {
				SetState(STATE_IDLE);
			}
			else {
				anim->animdelay = 0;
			}
			y = curGroundY - (height / 2) + HIT_GROUND_MARGIN;
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

			movey = 0;
		}
		else {
			if (shooting) {
				anim->ChangeAnimFrames(53, 55);
			}
			else {
				if (!(anim->curframe >= 48 && anim->curframe <= 50)) {
					anim->ChangeAnimFrames(48, 50);
				}
			}

			movey = -WALL_SLIDE_SPEED;
			if (delta_t % 2 == 0) {
				Effects::CreateSmoke(x + dirRight * (width / 2), y + height / 2 - 5, 0);
			}

			delta_t++;

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
			if (shooting) {
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
			if (shooting) {
				anim->ChangeAnimFrames(56, 57);
			}
			else {
				if (!(anim->curframe >= 51 && anim->curframe <= 52)) {
					anim->ChangeAnimFrames(51, 52);
				}
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
			if (shooting) {
				//SetState(STATE_DASHING_N_SHOOTING);
				anim->ChangeAnimFrames(62, 63);

			}
			else if (!shooting) {
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
				if (delta_t % 2 == 0) {
					Effects::CreateSmoke(x - dirRight * (width / 4), y + height / 2 - 5, -3);
				}
			}
		}
	}
	else 
	if (!(HitWall())) {
		if (state == STATE_JUMPING) {																//ASCENDING
			//if shooting key is pressed then change animation
			if (shooting) {
				//SetState(STATE_DASHING_N_SHOOTING);
				anim->ChangeAnimFrames(41, 43);

			}
			else if (!shooting) {
				//SetState(STATE_DASHING);
				anim->ChangeAnimFrames(34, 36);
			}

			//check if velocity is negative (falling down)
			if (!jumpHold || movey > 0) {
				SetState(STATE_FALLING);
				if (shooting) {
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
			if (shooting && inMidAir) {
				//SetState(STATE_DASHING_N_SHOOTING);
				//anim->curframe = anim->curframe - anim->beginframe + 43;
				anim->ChangeAnimFrames(43, 47);

			}
			else if (!shooting && inMidAir) {
				//SetState(STATE_DASHING);
				//anim->curframe = anim->curframe - anim->beginframe + 36;
				anim->ChangeAnimFrames(36, 40);
			}

			//Check if megaman hit the ground
			//if (isHitGround || anim->curframe > anim->beginframe + 2) {
			if (HitGround() || anim->curframe == 47) {
				if (anim->curframe == anim->endframe && anim->animcount >= anim->animdelay) {
					SetState(STATE_IDLE);
				}
				else {
					anim->animdelay = 0;
					//anim->animcount = 1;
				}
				y = curGroundY - (height / 2) + HIT_GROUND_MARGIN;
				movey = 0;
				inMidAir = false;
				delta_t = 0;
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
		else if (shooting && moving && !CloseToWall()) {													//SHOOTING AND RUNNING

			if (StateChanged(STATE_RUNNING_N_SHOOTING))
				SetState(STATE_RUNNING_N_SHOOTING);

		}
		else if (Input::KeyDown(DIK_V) && !dashHold && !CloseToWall()) {												//START DASHING

			if (StateChanged(STATE_DASHING))
				SetState(STATE_DASHING);

			delta_t = 0;
			movex = DASH_SPEED;
		}
		else if (shooting) {															//SHOOTING
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

	if (Input::KeyDown(DIK_X)) {
		shootHold = true;
		energy_t++;
	}
	else {
		shootHold = false;
		energy_t = -1;
	}

	if (shootingAnimDelay > SHOOTING_ANIMATION_DELAY) {
		shooting = false;
		shootingAnimDelay = -1;
		
	}
	else if (shooting) {
		shootingAnimDelay++;
	}

	if (HitWall()) {
		if (x - width / 2 <= curLeftWallX)
			x = curLeftWallX + width / 2 + 2;
		else if (x + width / 2 >= curRightWallX)
			x = curRightWallX - width / 2 - 2;
	}

	if (HitCeil()) {
		y = curCeilY + height / 2 + HIT_CEIL_MARGIN;
	}

	if (HitGround()) {
		y = curGroundY - height / 2 + HIT_GROUND_MARGIN;
	}


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

	if (Input::KeyDown(DIK_T)) {
		GAMELOG("x: %d, y: %d", x, y);
	}

	if (energy_t > -1 && energy_t <= SHOOTING_ANIMATION_DELAY) {
		color = D3DCOLOR_ARGB(255, 255, 255, 255);
		Effects::CreateMegamanEnergy();
	}
	else {
		if (energy_t % 3 == 0) {
			color = D3DCOLOR_ARGB(255, 50, 120, 255);
		}
		else {
			color = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}

	if (!isVulnerable) {
		if (invulnerable_t > INVULNERABLE_TIME) {
			isVulnerable = true;
			invulnerable_t = -1;
		}
		else if (invulnerable_t % 3 == 0) {
			color = D3DCOLOR_ARGB(0, 255, 255, 255);
		}
		else {
			color = D3DCOLOR_ARGB(180, 255, 255, 255);
		}
		invulnerable_t++;
	}

	//isHitGround = false;
}

void Megaman::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex * dirRight, y + movey);
	D3DXVECTOR2 translate = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + translate;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	//center = D3DXVECTOR3(width / 2, height / 2, 0);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	
	MObject::Update();
	MObject::Render();

	if (energy_t > SHOOTING_ANIMATION_DELAY) {
		if (energy_t >= CHARGED_SHOT_LV1_TIME && energy_t < CHARGED_SHOT_LV2_TIME) {
			Effects::DrawMegamanEnergy(matrix, x, y, 1);
		}
		else if (energy_t >= CHARGED_SHOT_LV2_TIME) {
			Effects::DrawMegamanEnergy(matrix, x, y, 2);
		}
	}
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