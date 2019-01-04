#include "ChasingBee.h"



ChasingBee::ChasingBee(Megaman* _player, D3DXVECTOR2 _firePoint)
{
	tag = (char*)"enemyBullet";
	dmg = 2;

	player = _player;

	x = _firePoint.x;
	y = _firePoint.y;
	dirRight = player->x >= x ? 1 : -1;

	delta_t = 0;
	slowUpdate = 0;

	anim = new Animation(4, 0, 3, ANIM_DELAY);

	char s[50];
	for (int i = 0; i < 4; i++) {
		sprintf_s(s, "sprites/blast_hornet/minibee/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}

}


ChasingBee::~ChasingBee()
{
}

void ChasingBee::ChasePlayer() {
	//if (delta_t % 3 == 0) {
	//	if (x > player->x) {
	//		movex--;
	//	}
	//	else if (x < player->x) {
	//		movex++;
	//	}
	//	else {
	//		movex = 0;
	//	}

	//	if (y > player->y) {
	//		movey--;
	//	}
	//	else if (y < player->y) {
	//		movey++;
	//	}
	//	else {
	//		movey = 0;
	//	}
	//}
	
	if (delta_t % 5 == 0) {
		if (x >= player->x) {
			if (movex >= -FLY_SPEED)
				movex--;
		}
		else if (x < player->x) {
			if (movex <= FLY_SPEED)
				movex++;
		}

		if (y >= player->y) {
			if (movey >= -FLY_SPEED)
				movey--;
		}
		else if (y < player->y) {
			if (movey <= FLY_SPEED)
				movey++;
		}
	}
	

	//if (abs(movex) >= FLY_SPEED) {
	//	movex = FLY_SPEED;
	//}
	//if (abs(movey) >= FLY_SPEED) {
	//	movey = FLY_SPEED;
	//}
}

void ChasingBee::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == "megaman" || otherObj->tag == "megamanBulletLvl0" || otherObj->tag == "megamanBulletLvl1" || otherObj->tag == "megamanBulletLvl2") {
		isDestroyed = true;
	}
}

void ChasingBee::Update() {
	delta_t++;

	dirRight = player->x >= x ? 1 : -1;

	if (delta_t >= 300) {
		isDestroyed = true;
	}

	ChasePlayer();

	x += movex;
	y += movey;
}

void ChasingBee::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x, y);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	anim->Animate(matrix);
	SetWidthHeight();
}