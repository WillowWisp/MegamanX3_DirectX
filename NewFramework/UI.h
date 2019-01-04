#pragma once
#include "MObject.h"

#define HP_CHANGE_ANIM_DELAY 1
#define HP_BAR_RELATIVE_X 30
#define HP_BAR_RELATIVE_Y 135
#define BOSS_HP_CHANGE_ANIM_DELAY 1
#define BOSS_HP_BAR_RELATIVE_X 485
#define BOSS_HP_BAR_RELATIVE_Y 104

class UI
{
public:
	static MObject * HPBar;
	static bool isIncreasingHP;
	static int curHP;
	static MObject * BossHPBar;
	static bool isIncreasingBossHP;
	static int curBossHP;
	static bool showBossHPBar;

	UI();
	~UI();

	static void InitHPBar();
	static void ChangeHP(int newHP);
	static void InitBossHPBar();
	static void ChangeBossHP(int newBossHP);
	static void DeleteBossHPBar();
	static void UpdateUI();
	static void RenderUI();
};

