#include "HeadGunnerMissile.h"


HeadGunnerMissile::HeadGunnerMissile(MObject* _target, D3DXVECTOR2 _firePoint, int _dirRight)
{
	target = _target;
	x = _firePoint.x;
	y = _firePoint.y;
	dirRight = _dirRight;

	movex = 10 * dirRight;

	anim = new Animation(2, 0, 1, 1);
	char s[50];
	for (int i = 0; i < 2; i++) {
		sprintf_s(s, "sprites/head_gunner/missile/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
}

HeadGunnerMissile::~HeadGunnerMissile()
{
}

void HeadGunnerMissile::OnCollision(MObject *otherObj, char* sideCollided) {

}

void HeadGunnerMissile::Update() {
	if (y > target->y)
		movey = -1;
	else if (y < target->y)
		movey = 1;
	else
		movey = 0;
}

void HeadGunnerMissile::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, NULL, NULL,
		NULL, &combined);
	x += movex;
	y += movey;
	anim->Animate(matrix);
}