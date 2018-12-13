#pragma once
#include "Animation.h"
#include <d3dx9.h>
#include "Graphics.h"
#include <stdio.h>
#include <stdlib.h>

class MObject
{
protected:
	virtual void OnCollision(MObject *otherObj, char* sideCollided);
public:
	MObject();
	~MObject();

	//function prototypes
	void SetAnimState(int begin, int end, int delay); //set chỉ số frame đầu và frame cuối của animation
	void Update();
	bool StateChanged(int);
	bool HorizontalDirChanged(int);
	bool VerticalDirChanged(int);
	void ChangeDirHorizontal(); //đổi hướng theo chiều ngang
	void ChangeDirVertical(); //đổi huowgns theo chiều dọc
	void SetWidthHeight();

	RECT GetRect();
	void MoveXYToCorner();
	void MoveXYToCenter();
	//void SetSignedMoveX();
	//void SetUnsignedMoveX();
	//

	int x, y;
	int width, height;
  
	int movex, movey;
	int delta_t; //For physics related calculation

	D3DXMATRIX matrix; //ma trận để lưu vị trí, scale, ... của object
	D3DCOLOR color; //Một số state của object có dùng tới đổi màu

	int state;
	int prevState; 
	int dirUp, dirRight;

	Animation* anim;
};

