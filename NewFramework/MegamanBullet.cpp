#include "MegamanBullet.h"



MegamanBullet::MegamanBullet()
{
}


MegamanBullet::~MegamanBullet()
{
	delete this;
}

MegamanBullet::MegamanBullet(int _x, int _y, int _dirRight, int level)
{
	x = _x;
	y = _y;
	movex = MBULLET_SPEED;
	movey = 0;
	dirRight = _dirRight;
	chargedLevel = level;
	//state_t = -1;
	isDestroyed = false;


	switch (chargedLevel)
	{
	case 0:
		anim = new Animation(26, 0, 0, MBULLET_ANIM_DELAY);
		dmg = MBULLET_DMG_LVL0;
		break;
	case 1:
		anim = new Animation(26, 4, 5, MBULLET_ANIM_DELAY);
		dmg = MBULLET_DMG_LVL1;
		break;
	case 2:
		anim = new Animation(26, 14, 15, MBULLET_ANIM_DELAY);
		dmg = MBULLET_DMG_LVL2;
		break;
	default:
		anim = new Animation(26, 0, 0, MBULLET_ANIM_DELAY);
		dmg = MBULLET_DMG_LVL0;
		break;
	}

	//SetState(0);

	char s[50];
	for (int i = 0; i < 26; i++) {
		sprintf_s(s, "sprites/megaman/bullets/%d.png", i);
		//sprintf_s(s, "sprites/megaman/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
}

void MegamanBullet::SetState(int newState) {
	state = newState;
	state_t = -1;
	if (state == MBULLET_STATE_OUT) {
		switch (chargedLevel)
		{
		case 0:
			SetAnimState(0, 0, MBULLET_ANIM_DELAY);
			dmg = MBULLET_DMG_LVL0;
			break;
		case 1:
			SetAnimState(4, 5, MBULLET_ANIM_DELAY);
			dmg = MBULLET_DMG_LVL1;
			break;
		case 2:
			SetAnimState(14, 15, MBULLET_ANIM_DELAY);
			dmg = MBULLET_DMG_LVL2;
			break;
		default:
			SetAnimState(0, 0, MBULLET_ANIM_DELAY);
			dmg = MBULLET_DMG_LVL0;
			break;
		}
	}
	else if (state == MBULLET_STATE_FLYING) {
		switch (chargedLevel)
		{
		case 0:
			SetAnimState(0, 0, MBULLET_ANIM_DELAY - 1);
			break;
		case 1:
			SetAnimState(6, 11, MBULLET_ANIM_DELAY - 1);
			break;
		case 2:
			SetAnimState(16, 18, MBULLET_ANIM_DELAY - 1);
			break;
		default:
			SetAnimState(0, 0, MBULLET_ANIM_DELAY - 1);
			break;
		}
	}
	else if (state == MBULLET_STATE_VANISHING) {
		switch (chargedLevel)
		{
		case 0:
			SetAnimState(1, 3, MBULLET_ANIM_DELAY);
			break;
		case 1:
			SetAnimState(12, 13, MBULLET_ANIM_DELAY);
			break;
		case 2:
			SetAnimState(19, 21, MBULLET_ANIM_DELAY);
			break;
		default:
			SetAnimState(1, 3, MBULLET_ANIM_DELAY);
			break;
		}
	}
}

void MegamanBullet::Update() {
	if (state == MBULLET_STATE_OUT) {
		if (state_t > MBULLET_OUT_ANIMATION_TIME) {
			SetState(MBULLET_STATE_FLYING);
		}
	}
	else if (state == MBULLET_STATE_FLYING) {
		if (state_t > MBULLET_FLYING_ANIMATION_TIME) {
			SetState(MBULLET_STATE_VANISHING);
		}
	}
	else if (state == MBULLET_STATE_VANISHING) {
		if (state_t > MBULLET_VANISHING_ANIMATION_TIME) {
			//
			isDestroyed = true;
		}
	}
	state_t++;

	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex * dirRight, y + movey);
	D3DXVECTOR2 translate = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + translate;

	D3DXVECTOR2 scale = D3DXVECTOR2(2 * dirRight, 2);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);

	MObject::Update();
}

void MegamanBullet::Vanish() {
	if (StateChanged(MBULLET_STATE_VANISHING))
		SetState(MBULLET_STATE_VANISHING);
}


