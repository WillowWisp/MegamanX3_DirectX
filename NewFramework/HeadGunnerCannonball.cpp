#include "HeadGunnerCannonball.h"
#include "GameLog.h"

HeadGunnerCannonball::HeadGunnerCannonball(MObject* _target, D3DXVECTOR2 _firePoint, int _dirRight)
{
	tag = (char*)"enemyBullet";
	dmg = 2;

	target = _target;
	x = _firePoint.x;
	y = _firePoint.y;
	dirRight = _dirRight;

	//int temp = 0;
	//Lực hướng lên ban đầu
	movey = -15;
	//temp = static_cast<int>((-(float)movey + sqrt(static_cast<float>((float)movey * (float)movey - 2 * ((float)y - (float)target->y)))));

	movex = (abs(x - target->x) / 18) * dirRight;

	anim = new Animation(); 
	anim->sprite[0] = new Sprite((char*)"sprites/head_gunner/bullet/0.png");

	Effects::CreateSmoke(x, y);
}


HeadGunnerCannonball::~HeadGunnerCannonball()
{
}

void HeadGunnerCannonball::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == (char*)"static" || otherObj->tag == (char*)"megaman" || otherObj->tag == "megamanBulletLvl0" || otherObj->tag == "megamanBulletLvl1" || otherObj->tag == "megamanBulletLvl2") {
		//Tự hủy
		isDestroyed = true;
	}
}

void HeadGunnerCannonball::Update() {
	movey += 1;
	if (state_t > BULLET_EXIST_TIME) {
		//Thoi gian gioi han ton tai cua dan
		isDestroyed = true;
	}
	state_t++;
}

void HeadGunnerCannonball::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	x += movex;
	y += movey;
	anim->Animate(matrix);
}