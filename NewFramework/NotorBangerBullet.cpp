#include "NotorBangerBullet.h"



NotorBangerBullet::NotorBangerBullet()
{
	tag = (char*)"enemyBullet";
	dmg = 2;
}


NotorBangerBullet::~NotorBangerBullet()
{
}

NotorBangerBullet::NotorBangerBullet(D3DXVECTOR2 _firePoint, int _dirRight) {
	x = _firePoint.x;
	y = _firePoint.y;
	dirRight = _dirRight;

	tag = (char*)"enemyBullet";
	dmg = 2;

	anim = new Animation();
	anim->sprite[0] = new Sprite((char*)"sprites/notor_banger/bullet/0.png");

	Effects::CreateSmoke(x, y);
}

void NotorBangerBullet::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == (char*)"static" || otherObj->tag == (char*)"megaman" || otherObj->tag == "megamanBulletLvl0" || otherObj->tag == "megamanBulletLvl1" || otherObj->tag == "megamanBulletLvl2") {
		//Tự hủy
		isDestroyed = true;
	}
}

void NotorBangerBullet::Fly45() {
	movex = 15 * dirRight;
	movey = -15;
}

void NotorBangerBullet::Fly90() {
	movex = 3 * dirRight;
	movey = -25;
}

void NotorBangerBullet::Update() {
	movey += 1;

	if (state_t > BULLET_EXIST_TIME) {
		//Thoi gian gioi han ton tai cua dan
		isDestroyed = true;
	}
	state_t++;
}

void NotorBangerBullet::Render() {
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