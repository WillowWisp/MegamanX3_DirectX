#include "MiniBee.h"



MiniBee::MiniBee(D3DXVECTOR2 des, D3DXVECTOR2 _firePoint, int _dirRight)
{
	tag = (char*)"enemyBullet";
	dmg = 2;

	x = _firePoint.x;
	y = _firePoint.y;
	dirRight = _dirRight;
	
	anim = new Animation(4, 0, 3, ANIM_DELAY);

	char s[50];
	for (int i = 0; i < 4; i++) {
		sprintf_s(s, "sprites/blast_hornet/minibee/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}

	Goto(des, FLY_SPEED);
}


MiniBee::~MiniBee()
{
}

void MiniBee::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == "static") {
		movex = 0;
		movey = 0;
	}
	if (otherObj->tag == "megaman" || otherObj->tag == "megamanBulletLvl0" || otherObj->tag == "megamanBulletLvl1" || otherObj->tag == "megamanBulletLvl2") {
		isDestroyed = true;
	}
}

void MiniBee::Update() {
	delta_t++;

	if (delta_t >= 150) {
		isDestroyed = true;
	}

	x += movex;
	y += movey;
}

void MiniBee::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x, y);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	anim->Animate(matrix);
	SetWidthHeight();
}