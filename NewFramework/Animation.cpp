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

void Animation::Animate(D3DXMATRIX _matrix, D3DCOLOR _color, D3DXVECTOR3 _center)
{
	if (animcount > animdelay) {
		animcount = 0;
		NextFrame(curframe + 1);
	}
	else {
		animcount++;
	}

	sprite[curframe]->Draw(_matrix, _color, _center);
}

void Animation::NextFrameWithoutLoop(int newframe) {
	if (newframe > endframe || newframe < beginframe) //state changed or out of frame range
		return;
	curframe = newframe;
}

void Animation::AnimateWithoutLoop(D3DXMATRIX _matrix, D3DCOLOR _color) {
	if (animcount > animdelay) {
		animcount = 0;
		NextFrameWithoutLoop(curframe + 1);
	}
	else {
		animcount++;
	}

	sprite[curframe]->Draw(_matrix, _color);
}

void Animation::ChangeAnimFrames(int begin, int end) {
	curframe = curframe - beginframe + begin;
	beginframe = begin;
	endframe = end;
}

Animation::~Animation()
{
	for (int i = 0; i < totalFrameCount; i++) {
		delete sprite[i];
	}
	delete[] sprite;
}
