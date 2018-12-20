#pragma once
#include "MObject.h"

#define HP_CHANGE_ANIM_DELAY 1
#define HP_BAR_RELATIVE_X 30
#define HP_BAR_RELATIVE_Y 135

class UI
{
public:
	static MObject * HPBar;
	static bool isIncreasingHP;
	static int curHP;

	UI();
	~UI();

	static void InitHPBar();
	static void ChangeHP(int newHP);
	static void UpdateUI();
	static void RenderUI();
};

