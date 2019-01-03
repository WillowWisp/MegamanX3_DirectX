#include "UI.h"

MObject* UI::HPBar = NULL;
bool UI::isIncreasingHP = false;
int UI::curHP = 16;
MObject* UI::BossHPBar = NULL;
bool UI::isIncreasingBossHP = false;
int UI::curBossHP = 0;
bool UI::showBossHPBar = false;

UI::UI()
{
}


UI::~UI()
{
	delete HPBar;
}

void UI::InitHPBar() {
	HPBar = new MObject();
	HPBar->anim = new Animation(17, 16, 16, HP_CHANGE_ANIM_DELAY);
	HPBar->delta_t = -1;

	char s[50];
	for (int i = 0; i < 17; i++) {
		sprintf_s(s, "sprites/megaman/hp_bar/%d.png", i);
		HPBar->anim->sprite[i] = new Sprite(s);
	}

	HPBar->x = HP_BAR_RELATIVE_X;
	HPBar->y = HP_BAR_RELATIVE_Y;
	D3DXVECTOR2 translation = D3DXVECTOR2(HPBar->x, HPBar->y);
	D3DXVECTOR2 scale = D3DXVECTOR2(2, 2);
	D3DXMatrixTransformation2D(&(HPBar->matrix), NULL, 0, &scale, NULL,
		NULL, &translation);
}

void UI::ChangeHP(int newHP) {
	
	if (newHP < 0)
		newHP = 0;
	else if (newHP > 16)
		newHP = 16;
	int HP = HPBar->anim->curframe;
	if (newHP == HP) {
		isIncreasingHP = false;
		HPBar->delta_t = -1;
	}
	else if (newHP < HP) {
		HPBar->anim->ChangeAnimFrames(newHP, newHP);
		
	}
	else if (newHP > HP) {
		if (HPBar->delta_t > HP_CHANGE_ANIM_DELAY) {
			HPBar->anim->ChangeAnimFrames(HP + 1, HP + 1);
			HPBar->delta_t = 0;
		}
		else {
			HPBar->delta_t++;
		}
	}
}

void UI::InitBossHPBar() {
	BossHPBar = new MObject();
	BossHPBar->anim = new Animation(33, 0, 0, BOSS_HP_CHANGE_ANIM_DELAY);
	BossHPBar->delta_t = -1;

	char s[50];
	for (int i = 0; i < 33; i++) {
		sprintf_s(s, "sprites/boss_hp_bar/%d.png", i);
		BossHPBar->anim->sprite[i] = new Sprite(s);
	}

	BossHPBar->x = BOSS_HP_BAR_RELATIVE_X;
	BossHPBar->y = BOSS_HP_BAR_RELATIVE_Y;
	D3DXVECTOR2 translation = D3DXVECTOR2(BossHPBar->x, BossHPBar->y);
	D3DXVECTOR2 scale = D3DXVECTOR2(2, 2);
	D3DXMatrixTransformation2D(&(BossHPBar->matrix), NULL, 0, &scale, NULL,
		NULL, &translation);
}

void UI::ChangeBossHP(int newHP) {
	if (newHP < 0)
		newHP = 0;
	else if (newHP > 32)
		newHP = 32;
	int BossHP = BossHPBar->anim->curframe;
	if (newHP == BossHP) {
		isIncreasingBossHP = false;
		BossHPBar->delta_t = -1;
	}
	else if (newHP < BossHP) {
		BossHPBar->anim->ChangeAnimFrames(newHP, newHP);

	}
	else if (newHP > BossHP) {
		if (BossHPBar->delta_t > BOSS_HP_CHANGE_ANIM_DELAY) {
			BossHPBar->anim->ChangeAnimFrames(BossHP + 1, BossHP + 1);
			BossHPBar->delta_t = 0;
		}
		else {
			BossHPBar->delta_t++;
		}
	}
}

void UI::DeleteBossHPBar() {
	showBossHPBar = false;
	delete BossHPBar;
}

void UI::UpdateUI() {
	ChangeHP(curHP);
}

void UI::RenderUI() {
	HPBar->Render();
	if (showBossHPBar) {
		BossHPBar->Render();
	}
}