#pragma once
#include "Sprite.h"

class Animation
{
	void NextFrameWithoutLoop(int newframe);
public:
	Animation(int count = 1, int begin = 0, int end = 0, int delay = 1);
	//Animation(int, int);
	//Animation(int, int, int);
	~Animation();

	//function prototypes
	void NextFrame(int); // set curfrfame bằng frame kế tiếp trong animation
	void Animate(D3DXMATRIX _matrix, 
					D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255), 
					D3DXVECTOR3 _center = D3DXVECTOR3()); // chọn frame và draw
	void AnimateWithoutLoop(D3DXMATRIX _matrix, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	void ChangeAnimFrames(int begin, int end);

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

