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

	anim = new Animation();
	anim->sprite[0] = new Sprite((char*)"sprites/helit/bullet/0.png");
}

void HelitBullet::OnCollision(MObject *otherObj, char* sideCollided) {

}

void HelitBullet::Update() {
	delta_t++;
	if (delta_t % 3 == 0) {
		movex += 1 * dirRight;
	}
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