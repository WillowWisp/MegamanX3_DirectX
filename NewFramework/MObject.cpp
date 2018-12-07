#include "MObject.h"



MObject::MObject()
{
	state = 0;
	//imageCount = 1;

	//width = anim->sprite[anim->curframe]->width;
	//height = anim->sprite[anim->curframe]->height;
}

void MObject::SetAnimState(int begin, int end, int delay)
{
	//spriteState = new Sprite(begin, end, delay);
	anim->beginframe = begin;
	anim->endframe = end;
	anim->animdelay = delay;
	anim->animcount = 0;
	anim->curframe = begin;
}

void MObject::Update()
{
	x += movex * dirRight;
	y -= movey;
	anim->Animate(matrix);
}

bool MObject::StateChanged(int newState)
{
	return newState != state;
}

bool MObject::HorizontalDirChanged(int newDir)
{
	return newDir != dirRight;
}

bool MObject::VerticalDirChanged(int newDir)
{
	return newDir != dirUp;
}

void MObject::ChangeDirHorizontal()
{
	dirRight *= -1;
}

void MObject::ChangeDirVertical()
{
	dirUp *= -1;
}

RECT MObject::GetRect() {
	RECT bound;

	bound.left = x - width / 2;
	bound.right = x + width / 2;
	bound.top = y - height / 2;
	bound.bottom = y + height / 2;

	return bound;
}

MObject::~MObject()
{
}
