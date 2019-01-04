#include "HelitBullet.h"



HelitBullet::HelitBullet()
{
}


HelitBullet::~HelitBullet()
{
}

HelitBullet::HelitBullet(D3DXVECTOR2 _firePoint, int _dirRight)
{
	x = _firePoint.x;
	y = _firePoint.y;
	dirRight = _dirRight;

	movex = 0;
	movey = 0;

	dmg = 2;

	anim = new Animation();
	anim->sprite[0] = new Sprite((char*)"sprites/helit/bullet/0.png");

	Effects::CreateSmoke(x, y);
}

void HelitBullet::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == (char*)"static" || otherObj->tag == (char*)"megaman" || otherObj->tag == "megamanBulletLvl0" || otherObj->tag == "megamanBulletLvl1" || otherObj->tag == "megamanBulletLvl2") {
		//Tự hủy
		isDestroyed = true;
	}
}

void HelitBullet::Update() {
	delta_t++;
	if (delta_t % 3 == 0) {
		movex += 1 * dirRight;
	}

	if (state_t % 8 == 0) {
		Effects::CreateSmoke(x, y, 0);
	}

	state_t++;

	state_t++; //to limit exist time of bullet
}

void HelitBullet::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	x += movex;
	y += movey;
	anim->AnimateWithoutLoop(matrix);
}