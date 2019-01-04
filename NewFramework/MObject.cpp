#include "MObject.h"



MObject::MObject()
{
	state = 0;
	dirRight = 1;
	color = D3DCOLOR_ARGB(255, 255, 255, 255);
	center = D3DXVECTOR3();
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
	y += movey;
}

void MObject::Render() {
	anim->Animate(matrix, color, center);
	SetWidthHeight();
}

void MObject::SetWidthHeight() {
	if (anim != NULL) {
		width = anim->sprite[anim->curframe]->width;
		height = anim->sprite[anim->curframe]->height;
	}
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

void MObject::MoveXYToCorner() {
	//Hot fix collision
	SetWidthHeight();
	x -= width / 2;
	y -= height / 2;
}

void MObject::MoveXYToCenter() {
	//Hot fix collision
	SetWidthHeight();
	x += width / 2;
	y += height / 2;
}

void MObject::SetSignedMoveX() {
	if (dirRight == -1) {
		movex *= -1;
	}
}

void MObject::SetUnsignedMoveX() {
	movex = abs(movex);
}

void MObject::Goto(D3DXVECTOR2 des, int speed) {
	D3DXVECTOR2 force;

	force.x = des.x - x;
	force.y = des.y - y;

	float distance = GameGlobal::DistanceBetween(D3DXVECTOR2(x, y), des);
	movex = (force.x / distance) * speed;
	movey = (force.y / distance) * speed;
}


MObject::~MObject()
{
	delete anim;
}

void MObject::OnCollision(MObject *otherObj, char* sideCollided) {

}