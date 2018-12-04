#include "MObject.h"



MObject::MObject()
{
	state = 0;
	//imageCount = 1;
}

void MObject::setAnimState(int begin, int end, int delay)
{
	//spriteState = new Sprite(begin, end, delay);
	anim->beginframe = begin;
	anim->endframe = end;
	anim->animdelay = delay;
	anim->animcount = 0;
	anim->curframe = begin;
}

void MObject::update()
{
	anim->animate(matrix);
}

bool MObject::stateChanged(int newState)
{
	return newState != state;
}

bool MObject::horizontalDirChanged(int newDir)
{
	return newDir != dirRight;
}

bool MObject::verticalDirChanged(int newDir)
{
	return newDir != dirUp;
}

void MObject::changeDirHorizontal()
{
	dirRight *= -1;
}

void MObject::changeDirVertical()
{
	dirUp *= -1;
}

MObject::~MObject()
{
}
