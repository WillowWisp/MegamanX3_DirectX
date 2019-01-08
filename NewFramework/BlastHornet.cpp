#include "BlastHornet.h"
CSound *bossexplosion;

BlastHornet::BlastHornet(Megaman* _player, RECT _flyingRegion)
{
	bossexplosion = Sound::LoadSound((char*)"Resources/Sounds/Effects/explosion.wav");
	flyingRegion = _flyingRegion;
	CalculatePoints();
	cycleDuration = BASE_CYCLE_DURATION;

	wing = new BlastHornetWing(this);

	tag = (char*)"boss";
	player = _player;
	x = rightRight.x;
	y = rightRight.y;
	dirUp = 1;
	dirRight = -1;
	movex = 0;
	movey = 0;

	phase = 0;
	HP = BH_TOTAL_HP;
	//HP = 1;

	invulnerable_t = -1;
	isVulnerable = true;

	delta_t = 0;

	anim = new Animation(20, 0, 0, ANIM_DELAY);
	char s[50];
	for (int i = 0; i < 20; i++) {
		sprintf_s(s, "sprites/blast_hornet/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}

	SetState(STATE_BH_FLYING);
}

BlastHornet::~BlastHornet()
{
}

void BlastHornet::CalculatePoints() {
	leftTop.x = flyingRegion.left + (flyingRegion.right - flyingRegion.left) * 1 / 4;
	leftTop.y = flyingRegion.top;
	
	leftLeft.x = flyingRegion.left;
	leftLeft.y = flyingRegion.top + (flyingRegion.bottom - flyingRegion.top) / 2;

	leftBottom.x = flyingRegion.left + (flyingRegion.right - flyingRegion.left) * 1 / 4;
	leftBottom.y = flyingRegion.bottom;

	middle.x = flyingRegion.left + (flyingRegion.right - flyingRegion.left) / 2;
	middle.y = flyingRegion.top + (flyingRegion.bottom - flyingRegion.top) / 2;

	rightTop.x = flyingRegion.left + (flyingRegion.right - flyingRegion.left) * 3 / 4;
	rightTop.y = flyingRegion.top;

	rightRight.x = flyingRegion.right;
	rightRight.y = flyingRegion.top + (flyingRegion.bottom - flyingRegion.top) / 2;

	rightBottom.x = flyingRegion.left + (flyingRegion.right - flyingRegion.left) * 3 / 4;
	rightBottom.y = flyingRegion.bottom;
}

void BlastHornet::SetState(int newState) {
	state = newState;

	switch (newState) {
	case STATE_BH_FLYING:
		SetAnimState(0, 0, ANIM_DELAY);
		break;
	case STATE_BH_PREPARE_CHARGE:
		SetAnimState(1, 12, ANIM_DELAY);
		break;
	case STATE_BH_CHARGE:
		SetAnimState(6, 6, ANIM_DELAY);
		break;
	case STATE_BH_RETURN_FROM_CHARGE:
		SetAnimState(0, 0, ANIM_DELAY);
		break;
	case STATE_BH_PREPARE_SHOOT_CONE:
		SetAnimState(13, 16, ANIM_DELAY);
		break;
	case STATE_BH_SHOOT_CONE:
		SetAnimState(17, 18, ANIM_DELAY);
		break;
	case STATE_BH_PREPARE_SHOOT_CHASING:
		SetAnimState(0, 3, ANIM_DELAY);
		break;
	case STATE_BH_SHOOT_CHASING:
		SetAnimState(3, 4, ANIM_DELAY);
		break;
	case STATE_BH_DYING:
		SetAnimState(19, 19, ANIM_DELAY);
		UI::DeleteBossHPBar();
		delta_t2 = 0;
		phase = -1;
		player->color = D3DCOLOR_ARGB(255, 255, 255, 255);
		player->isControllable = false;
		player->isVulnerable = false;
		player->movex = 0;
		player->movey = 0;
		delete wing;
		movex = 0;
		movey = 0;
		color = D3DCOLOR_ARGB(255, 0, 255, 255);
		//UI::DeleteBossHPBar();
		break;
	}
}

void BlastHornet::OnCollision(MObject *otherObj, char* sideCollided) {
	if (phase == 1) {
		if (state == STATE_BH_CHARGE) {
			if (otherObj->tag == (char*)"static") {
				delta_t = 0;
				movex = 0;
				movey = 0;
				SetState(STATE_BH_RETURN_FROM_CHARGE);
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

void BlastHornet::TakeDmg(int damage) {
	if (!isVulnerable)
		return;
	HP -= damage;
	UI::ChangeBossHP(HP);
	if (HP <= 0) {
		HP = 0;
		SetState(STATE_BH_DYING);
	}
	isVulnerable = false;
}

void BlastHornet::FaceDirRightTo(D3DXVECTOR2 point) {
	dirRight = point.x >= x ? 1 : -1;
}

void BlastHornet::ShootInCone() {
	firePoint = D3DXVECTOR2(x + dirRight * 55, y - 10);

	D3DXVECTOR2 playerPoint = D3DXVECTOR2(player->x, player->y);
	MiniBee* miniBee1 = new MiniBee(playerPoint, firePoint, dirRight);
	BulletsManager::CreateBullet(miniBee1);

	float distance = GameGlobal::DistanceBetween(D3DXVECTOR2(x, y), playerPoint);
	D3DXVECTOR2 vectorVuong = D3DXVECTOR2(-(playerPoint.y - y) / distance, (playerPoint.x - x) / distance);

	D3DXVECTOR2 point2 = D3DXVECTOR2(player->x + vectorVuong.x * BEE_SPREAD, player->y + vectorVuong.y * BEE_SPREAD);
	D3DXVECTOR2 point3 = D3DXVECTOR2(player->x - vectorVuong.x * BEE_SPREAD, player->y - vectorVuong.y * BEE_SPREAD);
	MiniBee* miniBee2 = new MiniBee(point2, firePoint, dirRight);
	BulletsManager::CreateBullet(miniBee2);
	MiniBee* miniBee3 = new MiniBee(point3, firePoint, dirRight);
	BulletsManager::CreateBullet(miniBee3);

	D3DXVECTOR2 point4 = D3DXVECTOR2(player->x + vectorVuong.x * BEE_SPREAD, player->y + vectorVuong.y * BEE_SPREAD * 2);
	D3DXVECTOR2 point5 = D3DXVECTOR2(player->x - vectorVuong.x * BEE_SPREAD, player->y - vectorVuong.y * BEE_SPREAD * 2);
	MiniBee* miniBee4 = new MiniBee(point4, firePoint, dirRight);
	BulletsManager::CreateBullet(miniBee4);
	MiniBee* miniBee5 = new MiniBee(point5, firePoint, dirRight);
	BulletsManager::CreateBullet(miniBee5);
}

void BlastHornet::ShootChasingBee() {
	firePoint = D3DXVECTOR2(x, y + 50);

	ChasingBee* bee = new ChasingBee(player, firePoint);
	BulletsManager::CreateBullet(bee);
}

float Map(float x, float x1, float x2, float y1, float y2) {
	//Map từ khoảng giá trị (x1, y1) sang (x2, y2)
	float m = (y2 - y1) / (x2 - x1);
	float c = y1 - m * x1;
	return m * x + c;
}

void BlastHornet::FlyInfinitySymbol() {
	//Tham khảo: Đức ngu - https://www.facebook.com/ptmdmusique
	//float t = Map(delta_t, 0, cycleDuration, dirRight == -1 ? 0 : -3.14, dirRight == -1 ? 3.14 : 0);
	float t = Map(delta_t, 0, cycleDuration, 0, 3.14);
	x = cos(t) * abs(flyingRegion.right - flyingRegion.left) / 2 + middle.x;
	y = (sin(t) * cos(t)) * abs(flyingRegion.bottom - flyingRegion.top) + middle.y;
}

void BlastHornet::Update() {
	delta_t++;

	if (phase == 0) {
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
			delta_t = 0;
			phase = 1;
			player->isControllable = true;
		}
	}
	
	if (phase == 1) {
		if (state == STATE_BH_FLYING) {
			FlyInfinitySymbol();
			FaceDirRightTo(D3DXVECTOR2(player->x, player->y));
			
			if (delta_t >= cycleDuration * 2) {
				delta_t = 0;
				SetState(STATE_BH_PREPARE_CHARGE);
			}
		}

		if (state == STATE_BH_PREPARE_CHARGE) {
			if (delta_t == 65) {
				delta_t = 0;
				SetState(STATE_BH_CHARGE);
				Goto(D3DXVECTOR2(player->x, player->y), 10);
			}
		}

		if (state == STATE_BH_RETURN_FROM_CHARGE) {
			FaceDirRightTo(rightRight);
			if (delta_t == 50) {
				Goto(rightRight, 7);
			}
			if ((x >= rightRight.x - 10 && x <= rightRight.x + 10 && y >= rightRight.y - 10 && y <= rightRight.y + 10)
				|| x >= flyingRegion.right) {
				movex = 0;
				movey = 0;
				x = rightRight.x;
				y = rightRight.y;
				delta_t = 0;
				SetState(STATE_BH_PREPARE_SHOOT_CONE);
			}
		}

		if (state == STATE_BH_PREPARE_SHOOT_CONE) {
			FaceDirRightTo(D3DXVECTOR2(player->x, player->y));
			if (delta_t == 50) {
				delta_t = 0;
				SetState(STATE_BH_SHOOT_CONE);
				ShootInCone();
			}
		}

		if (state == STATE_BH_SHOOT_CONE) {
			if (delta_t == 50) {
				delta_t = 0;
				if (HP <= BH_TOTAL_HP / 2) {
					cycleDuration = BASE_CYCLE_DURATION * 90 / 100;
					phase = 2;
					delta_t2 = 0;
					SetState(STATE_BH_FLYING);
				}
				else {
					SetState(STATE_BH_FLYING);
				}
			}
		}
	}
	
	if (phase == 2) {
		delta_t2++;

		FaceDirRightTo(D3DXVECTOR2(player->x, player->y));
		FlyInfinitySymbol();
		if (delta_t % cycleDuration == 0) {
			delta_t2 = 0;
			SetState(STATE_BH_PREPARE_SHOOT_CHASING);
		}

		if (state == STATE_BH_PREPARE_SHOOT_CHASING) {
			if (delta_t2 == 15) {
				delta_t2 = 0;
				SetState(STATE_BH_SHOOT_CHASING);
				ShootChasingBee();
			}
		}

		if (state == STATE_BH_SHOOT_CHASING) {
			if (delta_t2 == 15) {
				delta_t2 = 0;
				SetState(STATE_BH_FLYING);
			}
		}
	}


	if (state == STATE_BH_DYING) {
		if (delta_t2 > 255) {
			isDestroyed = true;
			player->isControllable = true;
			player->isVulnerable = true;
		}
		else {
			color = D3DCOLOR_ARGB(255 - delta_t2, 0, 255, 255);
			if (delta_t2 % 7 == 0 && delta_t2 < 255 - 30) {
				Effects::CreateExplosion(x + Random::RandInt(-30, 30),
					y + Random::RandInt(-50, 50));
				Sound::PlaySoundA(bossexplosion);
			}
			delta_t2++;
			anim->animcount = 0;
			player->anim->animcount = 0;
		}
	}



	if (!isVulnerable && !(state == STATE_BH_DYING)) {
		if (invulnerable_t > 20) {
			isVulnerable = true;
			invulnerable_t = -1;
			color = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
		else if (invulnerable_t % 3 == 0) {
			color = D3DCOLOR_ARGB(255, 0, 255, 255);
		}
		else {
			color = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
		invulnerable_t++;
	}

	x += movex;
	y += movey;

	if (state != STATE_BH_DYING)
		wing->Update();
}

void BlastHornet::Render() {
	if (state != STATE_BH_DYING)
		wing->Render();
	D3DXVECTOR2 translation = D3DXVECTOR2(x, y);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	anim->AnimateWithoutLoop(matrix, color);
	SetWidthHeight();
}