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
	void setAnimState(int, int, int); //set chỉ số frame đầu và frame cuối của animation
	void update();
	bool stateChanged(int);
	bool horizontalDirChanged(int);
	bool verticalDirChanged(int);
	void changeDirHorizontal(); //đổi hướng theo chiều ngang
	void changeDirVertical(); //đổi huowgns theo chiều dọc

	//

	int x, y;

	//int width, height;
	int movex, movey;

	D3DXMATRIX matrix; //ma trận để lưu vị trí, scale, ... của object

	int state;
	int dirUp, dirRight;

	Animation* anim;
};

