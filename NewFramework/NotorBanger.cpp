#include "NotorBanger.h"



NotorBanger::NotorBanger(MObject* _player, int _x, int _y)
{
	player = _player;

	tag = (char*)"enemy";
	HP = 3;
	x = _x;
	y = _y;
	movex = 0;
	movey = 10;
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
	//abc
}

void NotorBanger::Shoot45() {

	NotorBangerBullet* bullet = new NotorBangerBullet(firePoint, dirRight);
	//bulletList.push_back(bullet);
	BulletsManager::CreateBullet(bullet);
	bullet->Fly45();
}

void NotorBanger::Shoot90() {

	NotorBangerBullet* bullet = new NotorBangerBullet(firePoint, dirRight);
	//bulletList.push_back(bullet);
	BulletsManager::CreateBullet(bullet);
	bullet->Fly90();
}

void NotorBanger::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == (char*)"static") { //Nếu đụng vô vật static
		if (sideCollided == "left" || sideCollided == "right")
			movex = 0;
		if (sideCollided == "top") {
			delta_t = 0; //Chạm đất thì reset chu kì
			movex = 0;
			movey = 0;
			y = otherObj->y - otherObj->height / 2 - height / 2;
		}
		if (sideCollided == "bottom") {
			movex = 0;
			movey = 0;
			y = otherObj->y + otherObj->height / 2 + height / 2;
		}
	}
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

	//Quyết định hướng & góc bắn
	if (delta_t == 1) {
		dirRight = (x > player->x) ? -1 : 1; //Quay đầu theo hướng player
		//Nếu player đứng xa thì bắn góc 45 độ
		if (abs(x - player->x) >= 150) {
			angle = (char*)"45";
			firePoint = D3DXVECTOR2(x + dirRight * 25, y - 35);
		}
		//Nếu player đứng gẫn thì bắn góc 90 độ
		else {
			angle = (char*)"90";
			firePoint = D3DXVECTOR2(x - dirRight * 6, y - 45);
		}
	}

	if (angle == (char*)"45") {
		if (delta_t == 2) {		//Giơ nòng súng lên
			SetState(STATE_45_UP);
		}
		if (delta_t == 50) {	//Bắn
			SetState(STATE_45_SHOOT);
			Shoot45();
		}
		if (delta_t == 75) {	//Bắn
			SetState(STATE_45_SHOOT);
			Shoot45();
		}
		if (delta_t == 100) {	//Bắn
			SetState(STATE_45_SHOOT);
			Shoot45();
		}
		if (delta_t == 125) {	//Hạ nòng
			SetState(STATE_45_DOWN);
		}
	}
	
	if (angle == (char*)"90") {
		if (delta_t == 2) {		//Giơ nòng súng lên
			SetState(STATE_90_UP);
		}
		if (delta_t == 50) {	//Bắn
			SetState(STATE_90_SHOOT);
			Shoot90();
		}
		if (delta_t == 75) {	//Bắn
			SetState(STATE_90_SHOOT);
			Shoot90();
		}
		if (delta_t == 100) {	//Bắn
			SetState(STATE_90_SHOOT);
			Shoot90();
		}
		if (delta_t == 125) {	//Hạ nòng
			SetState(STATE_90_DOWN);
		}
	}
	

	if (delta_t == 150) {	//Lấy đà
		dirRight = (x > player->x) ? -1 : 1;
		SetState(STATE_PREPARE);
	}
	if (delta_t >= 160) {	//Nhảy
		if (state != STATE_JUMP) {
			SetState(STATE_JUMP);
			movex = 8 * dirRight;
			movey = -15;
		}
		movey = movey + 1;
	}

	//for (int i = 0; i < bulletList.size(); i++) {
	//	bulletList.at(i)->Update();
	//}
}

void NotorBanger::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex * dirRight, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	x += movex;
	y += movey;
	anim->AnimateWithoutLoop(matrix);
	SetWidthHeight();
}

