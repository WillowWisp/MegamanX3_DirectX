#include "Helit.h"

Helit::Helit(MObject* _player, int _x, int _y, int _dirRight, int _minHeight, int _maxHeight)
{
	player = _player;
	x = _x;
	y = _y;
	wing = new HelitWing(this);
	dirUp = 1;
	dirRight = _dirRight;
	movex = 0;
	movey = 0;

	flyUp = 1;
	if (_minHeight != -100000 && _maxHeight != -100000) {
		maxHeight = _maxHeight;
		minHeight = _minHeight;
	}
	else {
		maxHeight = y - 50;
		minHeight = y + 50;
	}

	bulletLeft = 2;

	HP = 4;

	delta_t = 0;

	anim = new Animation(2, 0, 0, ANIM_DELAY);
	char s[50];
	for (int i = 0; i < 2; i++) {
		sprintf_s(s, "sprites/helit/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
	
	SetState(STATE_IDLE);
}

Helit::~Helit()
{
	delete wing;
}

void Helit::Shoot() {
	bulletLeft--;
	firePoint = D3DXVECTOR2(x + 5 * dirRight, y + 20);
	HelitBullet* bullet = new HelitBullet(firePoint, dirRight);
	//bulletList.push_back(bullet);
	BulletsManager::CreateBullet(bullet);
}

void Helit::SetState(int newState) {
	state = newState;

	switch (newState) {
	case STATE_IDLE:
		SetAnimState(0, 0, ANIM_DELAY);
		break;
	case STATE_BH_SHOOT_CONE:
		SetAnimState(1, 1, ANIM_DELAY);
		break;
	case STATE_FLY_AWAY:
		SetAnimState(0, 0, ANIM_DELAY);
		break;
	}
}

void Helit::OnCollision(MObject *otherObj, char* sideCollided) {

}

void Helit::Update() {
	if (state == STATE_IDLE) { //Nếu đang IDLE thì mới di chuyển
		if (flyUp == 1) { //Nếu đang bay lên
			if (y > maxHeight) {
				movey = -4;
			}
			else {
				flyUp = -1;
			}
		}

		if (flyUp == -1) { //Nếu đang bay xuống
			if (y <= minHeight) {
				movey = 4;
			}
			else {
				flyUp = 1;
			}
		}

		if (player->y >= y - 5 && player->y <= y + 5) { //Nếu người chơi ngang độ cao vs Helit
			if (x * dirRight < player->x * dirRight) { //và Helit quay mặt vô ng chơi
				if (state != STATE_BH_SHOOT_CONE) {
					SetState(STATE_BH_SHOOT_CONE);
					Shoot();
					movey = 0;
					delta_t = 0; //Sau khi bắn thì hoãn 1 thời gian rồi mới di chuyển tiếp
				}
			}
		}
	}

	if (state == STATE_BH_SHOOT_CONE) {
		if (delta_t >= 50) {
			if (bulletLeft > 0)
				SetState(STATE_IDLE);
			else {
				SetState(STATE_FLY_AWAY);
				slowUpdate = 0;
			}
		}
		else {
			delta_t++;
		}
	}

	if (state == STATE_FLY_AWAY) {
		slowUpdate++;
		if (slowUpdate % 4 == 0)
			movey -= 1;
	}

	wing->Update();

	//for (auto bullet : bulletList) {
	//	bullet->Update();
	//}
}

void Helit::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	x += movex;
	y += movey;
	anim->Animate(matrix);

	wing->Render();

	//for (auto bullet : bulletList) {
	//	bullet->Render();
	//}
}