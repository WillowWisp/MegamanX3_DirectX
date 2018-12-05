#include "Animation.h"


Animation::Animation(int count, int begin, int end, int delay)
{
	totalFrameCount = count;
	animcount = 0;
	animdelay = delay;
	beginframe = begin;
	endframe = end;
	curframe = beginframe;

	sprite = new Sprite*[count];
}

void Animation::NextFrame(int newframe)
{
	if (newframe > endframe || newframe < beginframe) {
		//state changed or out of frame range
		curframe = beginframe;
	}
	else {
		curframe = newframe;
	}
}

void Animation::Animate(D3DXMATRIX _matrix)
{
	if (animcount > animdelay) {
		animcount = 0;
		NextFrame(curframe + 1);
	}
	else {
		animcount++;
	}

	sprite[curframe]->Draw(_matrix);
}

Animation::~Animation()
{
	delete sprite;
}
