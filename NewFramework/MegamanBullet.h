#pragma once
#include "Bullet.h"

#define MBULLET_STATE_OUT 0
#define MBULLET_STATE_FLYING 1
#define MBULLET_STATE_VANISHING 2
#define MBULLET_DMG_LVL0 10
#define MBULLET_DMG_LVL1 50
#define MBULLET_DMG_LVL2 100
#define MBULLET_OUT_ANIMATION_TIME 5
#define MBULLET_FLYING_ANIMATION_TIME 30
#define MBULLET_VANISHING_ANIMATION_TIME 5
#define MBULLET_SPEED 20
#define MBULLET_ANIM_DELAY 1

class MegamanBullet :
	public Bullet
{
private:
	int chargedLevel;
	int state_t;
public:

	MegamanBullet();
	~MegamanBullet();

	MegamanBullet(int _x, int _y, int _dirRight, int level);
	void SetState(int newState);
	void Update();
	void Vanish();
};

