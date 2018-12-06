#pragma once
#include "Animation.h"
#include <d3dx9.h>
#include "Graphics.h"
#include <stdio.h>
#include <stdlib.h>

class MObject
{
public:
	MObject();
	~MObject();

	//function prototypes
	void SetAnimState(int, int, int); //set chỉ số frame đầu và frame cuối của animation
	void Update();
	bool StateChanged(int);
	bool HorizontalDirChanged(int);
	bool VerticalDirChanged(int);
	void ChangeDirHorizontal(); //đổi hướng theo chiều ngang
	void ChangeDirVertical(); //đổi huowgns theo chiều dọc

	RECT GetRect();

	//

	int x, y;

	int width, height;

	//int width, height;
	int movex, movey;

	D3DXMATRIX matrix; //ma trận để lưu vị trí, scale, ... của object

	int state;
	int dirUp, dirRight;

	Animation* anim;
};

