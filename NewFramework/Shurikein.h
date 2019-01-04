#pragma once

#include "MObject.h"
#include "Megaman.h"
#include "Enemy.h"
#include "GameLog.h"
#include "Random.h"

#define ANIM_DELAY -1

#pragma region State
#define STATE_SHURIKEIN_DYING -1
//PHASE 0
#define STATE_SHURIKEIN_APPEARING 0
//PHASE 1
#define STATE_SHURIKEIN_PHASE1_ROLL 1
#define STATE_SHURIKEIN_PHASE1_JUMP 2
//PHASE 2
#define STATE_SHURIKEIN_PHASE2 3
//PHASE 3
#define STATE_SHURIKEIN_PHASE3 4
#pragma endregion

#pragma region Speed & stuff
#define PHASE1_SPEED_X 12
#define PHASE2_SPEED 7
#pragma endregion


class Shurikein : public Enemy
{
	int invulnerable_t;
	bool isVulnerable;

	int phase; //Shurikein có 3 phase 1,2,3. phase = 0 là lúc xuất hiện

	int phase1Count,
		phase2Count,
		phase3Count;

	Megaman* player;

	void SetState(int newState);
public:
	Shurikein(Megaman* _player, int _x, int _y, int _dirRight);
	~Shurikein();

	void TakeDmg(int damage);

	void Update();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
};

