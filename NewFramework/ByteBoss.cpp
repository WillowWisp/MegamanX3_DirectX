#include "ByteBoss.h"



ByteBoss::ByteBoss()
{
}


ByteBoss::~ByteBoss()
{
}

ByteBoss::ByteBoss(Megaman* _player, int _x, int _y) {
	player = _player;

	tag = (char*)"boss";
	HP = 32;
	x = _x;
	y = _y;
	//movex = 0;
	//movey = 0;
	dirUp = 1;
	dirRight = -1;

	delta_t = 0;
	anim = new Animation(23, 0, 0, BYTE_ANIM_DELAY);
	char s[50];
	for (int i = 0; i < 23; i++) {
		sprintf_s(s, "sprites/byte/scale2/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
	SetState(BYTE_STATE_APPEARING);
}

void ByteBoss::OnCollision(MObject *otherObj, char* sideCollided) {
	if (otherObj->tag == (char*)"static") {
		if (state == BYTE_STATE_APPEARING) {
			if (sideCollided == "top") {
				delta_t = 0;
				y = otherObj->y - otherObj->height / 2 - height / 2 + 1;
				movex = 0;
				movey = 0;
				//SetState(BYTE_STATE_IDLE);
				groundY = otherObj->y - otherObj->height / 2 + 1;
			}
		}
		else {
			if (sideCollided == "left" || sideCollided == "right") {
				movex = 0;
				if (sideCollided == "left") {
					x = otherObj->x - otherObj->width / 2 - width / 2 - BYTE_CHARGING_SPEED - 1;
				}
				else {
					x = otherObj->x + otherObj->width / 2 + width / 2 + BYTE_CHARGING_SPEED + 1;
				}
				ChangeDirHorizontal();
				SetState(BYTE_STATE_IDLE);
			}
			if (sideCollided == "top") {
				delta_t = 0;
				movex = 0;
				movey = 0;
				y = otherObj->y - otherObj->height / 2 - height / 2;
			}
			if (sideCollided == "bottom") {
				movex = 0;
				movey = 0;
				y = otherObj->y + otherObj->height / 2 + height / 2;
			}
		}
	}
	else if (otherObj->tag == (char*)"megaman") {
		if (state != BYTE_STATE_CHARGING_AT_PLAYER
			&& state != BYTE_STATE_PUNCHING_TO_CEIL
			&& state != BYTE_STATE_PUNCHING_TO_WALL)
		{
			player->TakeDmg(3);
		}
		else {
			player->TakeDmg(5);
		}
	}
}

void ByteBoss::SetState(int newState) {
	state = newState;
	delta_t = 0;
	switch (newState)
	{
	case BYTE_STATE_APPEARING:
		SetAnimState(0, 0, BYTE_ANIM_DELAY);
		movex = 0;
		movey = BYTE_DESCENDING_SPEED;
		//UI::curBossHP = 0;
		//UI::ChangeBossHP(UI::curBossHP);
		break;
	case BYTE_STATE_IDLE:
		SetAnimState(0, 0, BYTE_ANIM_DELAY);
		movex = 0;
		movey = 0;
		break;
	case BYTE_STATE_THROWING_MINE:
		SetAnimState(1, 4, BYTE_ANIM_DELAY + 1);
		movex = 0;
		movey = 0;
		break;
	case BYTE_STATE_PREPARING:
		SetAnimState(0, 0, BYTE_ANIM_DELAY);
		movex = 0;
		movey = 0;
		break;
	case BYTE_STATE_CHARGING_AT_PLAYER:
		SetAnimState(5, 8, BYTE_ANIM_DELAY + 1);
		movex = 0;
		movey = 0;
		break;
	case BYTE_STATE_PUNCHING_TO_CEIL:
		SetAnimState(9, 10, BYTE_ANIM_DELAY);
		movex = 0;
		movey = 0;
		break;
	case BYTE_STATE_PUNCHING_TO_WALL:
		SetAnimState(11, 22, BYTE_ANIM_DELAY);
		movex = 0;
		movey = 0;
		break;
	default:
		break;
	}
}

void ByteBoss::AdjustPosition() {
	y = groundY - height / 2;

	GAMELOG("y - h/2: %d\nground: %d", y + height / 2, groundY);

}

void ByteBoss::SetRenderXY() {
	renderX = x + (x + 184 / 2) - (x + width / 2);
	renderY = y;
}

void ByteBoss::Update() {
	if (state == BYTE_STATE_APPEARING) {
		if (movey == 0) {
			int curBossHP = UI::BossHPBar->anim->curframe;
			if (curBossHP == 0) { //Hp bar just appeared
				player->isControllable = false;
				player->movex = 0;
				player->movey = 0;
				player->anim->animcount = 0;
				UI::isIncreasingBossHP = true;
				UI::showBossHPBar = true;
				//UI::curBossHP++;
				UI::ChangeBossHP(HP);
			}
			else if (curBossHP < HP) {
				//UI::curBossHP++;
				UI::ChangeBossHP(HP);
				player->anim->animcount = 0;
				//UI::curBossHP++;
			}
			else {
				//UI::ChangeBossHP(UI::curBossHP);
				SetState(BYTE_STATE_IDLE);
				player->isControllable = true;
			}
		}
	}
	else if (state == BYTE_STATE_IDLE) {
		if (delta_t > BYTE_STATE_TIME_IDLE) {
			SetState(BYTE_STATE_THROWING_MINE);
			x -= 20 * (-dirRight);
		}
		AdjustPosition();
		
	}
	else if (state == BYTE_STATE_THROWING_MINE) {
		if (anim->curframe == anim->endframe
			&& anim->animcount >= anim->animdelay)
		{
			SetState(BYTE_STATE_PREPARING);
			x += 20 * (-dirRight);
		}
		else {
			AdjustPosition();
		}
	}
	else if (state == BYTE_STATE_PREPARING) {
		if (delta_t > BYTE_STATE_TIME_PREPARING) {
			SetState(BYTE_STATE_CHARGING_AT_PLAYER);
			//x -= 20;
		}
		AdjustPosition();
	}
	else if (state == BYTE_STATE_CHARGING_AT_PLAYER) {
		if (anim->curframe == anim->endframe) {
			anim->animcount = 0;
			movex = BYTE_CHARGING_SPEED;
			if (delta_t % 7 == 0) {
				Effects::CreateSmoke(x - dirRight * (width / 2), y + height / 2 - 5, -3);
			}
		}
		AdjustPosition();
	}
	//AdjustPosition();
	delta_t++;
	MObject::Update();
}

void ByteBoss::Render() {
	//SetRenderXY();
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex * dirRight, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(-1 * dirRight, 1);
	//center = D3DXVECTOR3(width / 2, height / 2, 0);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);

	MObject::Render();
}