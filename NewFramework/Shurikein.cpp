#include "Shurikein.h"

CSound *shurikenexplosion;

Shurikein::Shurikein(Megaman* _player, int _x, int _y, int _dirRight)
{
	shurikenexplosion= Sound::LoadSound((char*)"Resources/Sounds/Effects/explosion.wav");
	player = _player;
	x = _x;
	y = _y;
	dirRight = _dirRight;
	dirUp = 1;
	movex = 0;
	movey = 0;
	delta_t = 0;

	invulnerable_t = -1;
	isVulnerable = true;

	HP = 30;

	phase = 0;
	phase1Count = 3;
	phase2Count = 1;
	phase3Count = 7;

	anim = new Animation(106, 0, 41, ANIM_DELAY);
	char s[50];
	for (int i = 0; i < 106; i++) {
		sprintf_s(s, "sprites/shurikein/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
}


Shurikein::~Shurikein()
{
}

void Shurikein::SetState(int newState) {
	state = newState;
	switch (newState) {
	case STATE_SHURIKEIN_APPEARING:
		SetAnimState(0, 41, ANIM_DELAY);
		break;
	case STATE_SHURIKEIN_PHASE1_ROLL:
		phase = 1;
		SetAnimState(84, 105, ANIM_DELAY);
		break;
	case STATE_SHURIKEIN_PHASE1_JUMP:
		//SetAnimState(0, 0, ANIM_DELAY);
		break;
	case STATE_SHURIKEIN_PHASE2:
		phase = 2;
		SetAnimState(41, 51, ANIM_DELAY);
		break;
	case STATE_SHURIKEIN_PHASE3:
		phase = 3;
		SetAnimState(52, 83, ANIM_DELAY);
		break;
	case STATE_SHURIKEIN_DYING:
		phase = -1;
		SetAnimState(84, 84, ANIM_DELAY);
		player->isControllable = false;
		player->movex = 0;
		player->movey = 0;
		movex = 0;
		movey = 0;
		break;
	}
}


void Shurikein::OnCollision(MObject *otherObj, char* sideCollided) {
	if (phase == 1) {
		if (otherObj->tag == "static") {
			if (sideCollided == (char*)"right") {
				dirRight = 1;
			}
			if (sideCollided == (char*)"left") {
				dirRight = -1;

				phase1Count--;
			}
			if (sideCollided == (char*)"top") {
				movey = 0;
				state = STATE_SHURIKEIN_PHASE1_ROLL;
			}
		}

		if (state == STATE_SHURIKEIN_PHASE1_ROLL && movex < 0) {
			if (otherObj->tag == "megamanBulletLvl0" || otherObj->tag == "megamanBulletLvl1" || otherObj->tag == "megamanBulletLvl2") {
				movey = -25;
				SetState(STATE_SHURIKEIN_PHASE1_JUMP);
			}
		}
	}

	if (phase == 2) {
		if (otherObj->tag == "static") {
			if (sideCollided == (char*)"right") {
				movex = 0;
				movey = PHASE2_SPEED * -1;
			}
			if (sideCollided == (char*)"bottom") {
				movex = PHASE2_SPEED;
				movey = 0;
			}
			if (sideCollided == (char*)"left") {
				movex = 0;
				movey = PHASE2_SPEED;
			}
			if (sideCollided == (char*)"top") {
				movex = PHASE2_SPEED * -1;
				movey = 0;

				phase2Count--;
			}
		}
	}

	if (phase == 3) {
		if (otherObj->tag == "static") {
			if (sideCollided == "right" || sideCollided == "left") {
				dirRight *= -1;
			}
			if (sideCollided == "top") {
				movey = -1 * Random::RandInt(10, 30);
				//movey = -20;

				phase3Count--;
			}
		}
	}

	if (otherObj->tag == (char*)"megaman" && player->isVulnerable) {
		player->TakeDmg(3);
	}

	if (otherObj->tag == (char*)"megamanBulletLvl0" || otherObj->tag == (char*)"megamanBulletLvl1" || otherObj->tag == (char*)"megamanBulletLvl2") {
		if (otherObj->tag == (char*)"megamanBulletLvl0") {
			TakeDmg(1);
		}
		else if (otherObj->tag == (char*)"megamanBulletLvl1") {
			TakeDmg(2);
		}
		else {
			TakeDmg(5);
		}
	}
}

void Shurikein::TakeDmg(int damage) {
	if (!isVulnerable)
		return;
	HP -= damage;
	UI::ChangeBossHP(HP);
	if (HP <= 0) {
		HP = 0;
		delta_t = 0;
		SetState(STATE_SHURIKEIN_DYING);
	}
	isVulnerable = false;
}


void Shurikein::Update() {
	delta_t++;

	if (phase == 0) {
		if (state != STATE_SHURIKEIN_APPEARING) {
			SetState(STATE_SHURIKEIN_APPEARING);
		}

		if (delta_t >= 82) {
			delta_t = 0;
			phase = 1;
			SetState(STATE_SHURIKEIN_PHASE1_ROLL);
		}
	}

	if (phase == 1) {
		if (delta_t >= 150) {
			movex = 10 * dirRight;
		}
		if (state == STATE_SHURIKEIN_PHASE1_ROLL) {
			movey = 0;
		}
		if (state == STATE_SHURIKEIN_PHASE1_JUMP) {
			movey += 1;
		}

		//Tạm thời, sẽ gọi khi bị đạn bắn trúng lúc đang ROLL phase1
		//if (delta_t == 190) {
		//	movey = -20;
		//	SetState(STATE_PHASE1_JUMP);
		//}
	}

	if (phase == 2) {
		if (delta_t == 150) {
			movex = PHASE2_SPEED * -1;
			movey = 0;
			//Sẽ xử lý tiếp lúc va chạm
		}
	}

	if (phase == 3) {
		if (delta_t == 150) {
			dirRight = -1;
			movey = -1 * Random::RandInt(10, 30);
			//movey = -20; //Gia tri lực nhảy này sẽ random sau mỗi lần bật
			movex = 5 * dirRight;
		}
		if (delta_t > 150) {
			movey += 1;
			movex = 5 * dirRight;
		}
	}

	if (phase1Count <= 0) {
		phase1Count = 3;
		SetState(STATE_SHURIKEIN_PHASE2);
		phase = 2;
		delta_t = 0;
		movex = 0;
		//movey = 0;
	}
	if (phase2Count <= 0) {
		phase2Count = 1;
		SetState(STATE_SHURIKEIN_PHASE3);
		phase = 3;
		delta_t = 0;
		movex = 0;
		movey = 0;
	}
	if (phase3Count <= 0) {
		phase3Count = 6;
		SetState(STATE_SHURIKEIN_PHASE1_ROLL);
		phase = 1;
		delta_t = 0;
		movex = 0;
		movey = 0;
	}

	if (state == STATE_SHURIKEIN_DYING) {
		if (delta_t > 255) {
			isDestroyed = true;
			player->isControllable = true;
			player->isVulnerable = true;
		}
		else {
			color = D3DCOLOR_ARGB(255 - delta_t, 0, 255, 255);
			if (delta_t % 7 == 0 && delta_t < 255 - 30) {
				Effects::CreateExplosion(x + Random::RandInt(-30, 30),
					y + Random::RandInt(-50, 50));
				Sound::PlaySoundA(shurikenexplosion);
			}
			//delta_t++;
			anim->animcount = 0;
			player->anim->animcount = 0;
		}
	}

	if (!isVulnerable && !(state == STATE_SHURIKEIN_DYING)) {
		if (invulnerable_t > 10) {
			isVulnerable = true;
			invulnerable_t = -1;
			color = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
		else if (invulnerable_t % 3 == 0) {
			color = D3DCOLOR_ARGB(255, 255, 0, 0);
		}
		else {
			color = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
		invulnerable_t++;
	}
}

void Shurikein::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(-1 * dirRight, 1); //-1 do hình bị ngược
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	x += movex;
	y += movey;
	anim->Animate(matrix, color);
}