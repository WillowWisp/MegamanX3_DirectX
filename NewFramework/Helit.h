#pragma once

#include "MObject.h"
#include "HelitWing.h"
#include "HelitBullet.h"
#include <vector>
#include "Enemy.h"
#include "BulletsManager.h"

#define STATE_IDLE 0
#define STATE_BH_SHOOT_CONE 1
#define STATE_FLY_AWAY 2

class Helit : public Enemy
{
	HelitWing* wing; //Chứa hình ảnh cánh quạt riêng

	MObject* player; //Target
	int flyUp;
	int minHeight;
	int maxHeight;

	//std::vector<HelitBullet*> bulletList;
	D3DXVECTOR2 firePoint; //Điểm nhả đạn
	int bulletLeft;

	int slowUpdate; //Làm chậm 1 phần hàm Update lại bằng cách slowUpdate++ và if (slowUpdate % 3 == 0) { phần cần làm chậm }


	void SetState(int newState);

	void Shoot();
public:
	Helit(MObject* _player, int _x, int _y, int _dirRight = 1, int _minHeight = -100000, int _maxHeight = -100000);
	~Helit();

	void Update();
	void Render();
	void OnCollision(MObject *otherObj, char* sideCollided);
};

