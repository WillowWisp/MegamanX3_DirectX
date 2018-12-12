#include "NotorBangerBullet.h"



NotorBangerBullet::NotorBangerBullet()
{
}


NotorBangerBullet::~NotorBangerBullet()
{
}

NotorBangerBullet::NotorBangerBullet(int _x, int _y, int _dirRight) {
	x = _x;
	y = _y;
	dirRight = _dirRight;

	anim = new Animation();
	anim->sprite[0] = new Sprite((char*)"sprites/notor_banger/bullet/0.png");
}

void NotorBangerBullet::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == (char*)"static" || otherObj->tag == (char*)"player") {
		//Tự hủy
	}
}

void NotorBangerBullet::Fly45() {
	movex = 15 * dirRight;
	movey = -15;
}

void NotorBangerBullet::Update() {
	movey += 1;
}

void NotorBangerBullet::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, NULL, NULL,
		NULL, &combined);
	x += movex;
	y += movey;
	anim->AnimateWithoutLoop(matrix);
}