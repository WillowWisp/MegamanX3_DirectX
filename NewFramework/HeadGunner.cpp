#include "HeadGunner.h"



HeadGunner::HeadGunner(MObject* _player, int _x, int _y, int _dirRight)
{
	player = _player;
	x = _x;
	y = _y;

	movex = 0;
	movey = 0;
	dirUp = 1;
	dirRight = _dirRight;

	HP = 6;

	delta_t = 0;

	anim = new Animation(8, 0, 0, ANIM_DELAY);
	char s[50];
	for (int i = 0; i < 8; i++) {
		sprintf_s(s, "sprites/head_gunner/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
}

HeadGunner::~HeadGunner()
{
}

void HeadGunner::ShootMissile() {
	HeadGunnerMissile* missile = new HeadGunnerMissile(player, firePoint, dirRight);
	//bulletList.push_back(missile);
	BulletsManager::CreateBullet(missile);
}

void HeadGunner::ShootCannonball() {
	HeadGunnerCannonball* cannonball = new HeadGunnerCannonball(player, firePoint, dirRight);
	//bulletList.push_back(cannonball);
	BulletsManager::CreateBullet(cannonball);
}

void HeadGunner::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == (char*)"static") { //Nếu đụng vô vật static
		if (sideCollided == "top") {
			delta_t = 0; //Chạm đất thì reset chu kì
			movex = 0;
			movey = 0;
			y = otherObj->y - otherObj->height / 2 - height / 2;
		}
	}
}

void HeadGunner::SetState(int newState) {
	state = newState;

	switch (newState) {
	case STATE_IDLE:
		SetAnimState(0, 0, ANIM_DELAY);
		break;
	case STATE_PREPARE_SHOOT_1:
		SetAnimState(1, 2, ANIM_DELAY + 3);
		break;
	case STATE_SHOOT_1:
		SetAnimState(3, 3, ANIM_DELAY);
		break;
	case STATE_PREPARE_SHOOT_2:
		SetAnimState(4, 5, ANIM_DELAY + 3);
		break;
	case STATE_SHOOT_2:
		SetAnimState(6, 6, ANIM_DELAY);
		break;
	case STATE_SHOOT_CANON:
		SetAnimState(7, 7, ANIM_DELAY);
		break;
	}
}

void HeadGunner::Update() {
	delta_t++;

	if (delta_t == 5) {
		SetState(STATE_IDLE);
	}
	if (delta_t == 40) {
		SetState(STATE_PREPARE_SHOOT_1);
	}
	if (delta_t == 70) {
		SetState(STATE_SHOOT_1);
		firePoint = D3DXVECTOR2(x + dirRight * 25, y - 20);
		ShootMissile();
	}
	if (delta_t == 110) {
		SetState(STATE_PREPARE_SHOOT_2);
	}
	if (delta_t == 140) {
		SetState(STATE_SHOOT_2);
		firePoint = D3DXVECTOR2(x + dirRight * 25, y - 10);
		ShootMissile();
	}
	if (delta_t == 180) {
		SetState(STATE_SHOOT_CANON);
		firePoint = D3DXVECTOR2(x, y - 40);
		ShootCannonball();
	}
	if (delta_t == 220) {
		SetState(STATE_SHOOT_CANON);
		firePoint = D3DXVECTOR2(x, y - 40);
		ShootCannonball();
	}
	if (delta_t == 260) {
		SetState(STATE_SHOOT_CANON);
		firePoint = D3DXVECTOR2(x, y - 40);
		ShootCannonball();
	}
	if (delta_t >= 300)
		delta_t = 0;

	//for (int i = 0; i < bulletList.size(); i++) {
	//	bulletList.at(i)->Update();
	//}
}

void HeadGunner::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	x += movex;
	y += movey;
	anim->AnimateWithoutLoop(matrix);

	//for (int i = 0; i < bulletList.size(); i++) {
	//	bulletList.at(i)->Render();
	//}
}