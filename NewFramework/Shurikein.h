#pragma once

#include "MObject.h"
#include "Enemy.h"
#include "GameLog.h"

#define ANIM_DELAY -1

#pragma region State
//PHASE 0
#define STATE_APPEARING 0
//PHASE 1
#define STATE_PHASE1_ROLL 1
#define STATE_PHASE1_JUMP 2
//PHASE 2
#define STATE_PHASE2 3
//PHASE 3
#define STATE_PHASE3 4
#pragma endregion

#pragma region Speed & stuff
#define PHASE1_SPEED_X 12
#define PHASE2_SPEED 7
#pragma endregion


class Shurikein : public Enemy
{
	int phase; //Shurikein có 3 phase 1,2,3. phase = 0 là lúc xuất hiện

	int phase1Count,
		phase2Count,
		phase3Count;

	MObject* player;

	void SetState(int newState);
public:
	Shurikein(MObject* _player, int _x, int _y, int _dirRight);
	~Shurikein();

	void Update();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
};

