#pragma once
#include "Sprite.h"

class Animation
{
public:
	Animation(int count = 1, int begin = 0, int end = 0, int delay = 5);
	//Animation(int, int);
	//Animation(int, int, int);
	~Animation();

	//function prototypes
	void nextFrame(int); // set curfrfame bằng frame kế tiếp trong animation
	void animate(D3DXMATRIX); // chọn frame và draw

	//
	int totalFrameCount;
	int beginframe; // chỉ số frame đầu của animation hiện tại
	int curframe;	// chỉ số frame hiện tại
	int endframe;	// chỉ số frame cuối của animation hiện tại
	int animdelay;
	int animcount;
	Sprite** sprite; // một instance của class Animation lưu tất cả các sprite của một object 
					 // dưới dạng mảng những con trỏ sprite
};

