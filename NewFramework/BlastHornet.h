#pragma once

#include "MObject.h"
#include "Megaman.h"
#include "Enemy.h"
#include "UI.h"
#include "BulletsManager.h"
#include "GameGlobal.h"
#include "GameLog.h"
#include "BlastHornetWing.h"
#include "MiniBee.h"
#include "ChasingBee.h"

#define ANIM_DELAY 1

#define STATE_BH_FLYING 0
#define STATE_BH_PREPARE_CHARGE 1
#define STATE_BH_CHARGE 2
#define STATE_BH_RETURN_FROM_CHARGE 5
#define STATE_BH_PREPARE_SHOOT_CONE 3
#define STATE_BH_SHOOT_CONE 4
#define STATE_BH_PREPARE_SHOOT_CHASING 6
#define STATE_BH_SHOOT_CHASING 7
#define STATE_BH_DYING -1

#define BH_TOTAL_HP 32

#define BASE_CYCLE_DURATION 100
#define BEE_SPREAD 60

class BlastHornet : public Enemy
{
	int invulnerable_t;
	bool isVulnerable;

	#pragma region Bay hình số 8
	int cycleDuration;

	RECT flyingRegion;
	//Lấy 7 điểm trên hình số 8 (đường bay của boss)
	D3DXVECTOR2 leftTop, leftLeft, leftBottom;
	D3DXVECTOR2 middle;
	D3DXVECTOR2 rightTop, rightRight, rightBottom;

	void CalculatePoints(); //Tính tọa độ 7 điểm trên

	#pragma endregion

	Megaman* player; //Target
	int phase;

	int delta_t2;

	BlastHornetWing* wing;
	D3DXVECTOR2 firePoint; //Điểm nhả đạn

	void FaceDirRightTo(D3DXVECTOR2 point);

	void FlyInfinitySymbol();

	void SetState(int newState);

	void ShootInCone();

	void ShootChasingBee();
public:
	BlastHornet(Megaman* _player, RECT _flyingRegion);
	~BlastHornet();

	void TakeDmg(int damage);

	void Update();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
};

