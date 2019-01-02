#pragma once

#include "MObject.h"
#include "Enemy.h"
#include "BulletsManager.h"
#include "GameGlobal.h"

#define ANIM_DELAY 1

#define STATE_FLYING 0
#define STATE_PREPARE_CHARGE 1
#define STATE_CHARGE 2
#define STATE_PREPARE_SHOOT 3
#define STATE_SHOOT 4

class BlastHornet : public Enemy
{
	#pragma region Bay hình số 8

	RECT flyingRegion;
	//Lấy 7 điểm trên hình số 8 (đường bay của boss)
	D3DXVECTOR2 leftTop, leftLeft, leftBottom;
	D3DXVECTOR2 middle;
	D3DXVECTOR2 rightTop, rightRight, rightBottom;

	D3DXVECTOR2 pointsByOrder[8]; //Xếp các điểm theo thứ tự tạo thành đường bay của boss
	int currentPoint; //Điểm hiện tại mà boss vừa bay qua

	void CalculatePoints(); //Tính tọa độ 7 điểm trên

	#pragma endregion

	MObject* player; //Target
	int phase;

	D3DXVECTOR2 firePoint; //Điểm nhả đạn


	void SetState(int newState);

	void Shoot();
public:
	BlastHornet(MObject* _player, int _x, int _y, RECT _flyingRegion);
	~BlastHornet();

	void Update();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
};

