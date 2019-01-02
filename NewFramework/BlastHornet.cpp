#include "BlastHornet.h"


BlastHornet::BlastHornet(MObject* _player, int _x, int _y, RECT _flyingRegion)
{
	flyingRegion = _flyingRegion;
	CalculatePoints();
	currentPoint = 0;

	player = _player;
	x = rightRight.x;
	y = rightRight.y;
	dirUp = 1;
	movex = 0;
	movey = 0;

	phase = 0;
	HP = 100;

	delta_t = 0;

	anim = new Animation(19, 0, 0, ANIM_DELAY);
	char s[50];
	for (int i = 0; i < 19; i++) {
		sprintf_s(s, "sprites/blast_hornet/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}

	SetState(STATE_FLYING);
}

BlastHornet::~BlastHornet()
{
}

void BlastHornet::CalculatePoints() {
	leftTop.x = flyingRegion.left + (flyingRegion.right - flyingRegion.left) * 1 / 4;
	leftTop.y = flyingRegion.top;
	
	leftLeft.x = flyingRegion.left;
	leftLeft.y = flyingRegion.top + (flyingRegion.bottom - flyingRegion.top) / 2;

	leftBottom.x = flyingRegion.left + (flyingRegion.right - flyingRegion.left) * 1 / 4;
	leftBottom.y = flyingRegion.bottom;

	middle.x = flyingRegion.left + (flyingRegion.right - flyingRegion.left) / 2;
	middle.y = flyingRegion.top + (flyingRegion.bottom - flyingRegion.top) / 2;

	rightTop.x = flyingRegion.left + (flyingRegion.right - flyingRegion.left) * 3 / 4;
	rightTop.y = flyingRegion.top;

	rightRight.x = flyingRegion.right;
	rightRight.y = flyingRegion.top + (flyingRegion.bottom - flyingRegion.top) / 2;

	rightBottom.x = flyingRegion.left + (flyingRegion.right - flyingRegion.left) * 3 / 4;
	rightBottom.y = flyingRegion.bottom;

	pointsByOrder[0] = rightRight;
	pointsByOrder[1] = rightTop;
	pointsByOrder[2] = middle;
	pointsByOrder[3] = leftBottom;
	pointsByOrder[4] = leftLeft;
	pointsByOrder[5] = leftTop;
	pointsByOrder[6] = middle;
	pointsByOrder[7] = rightBottom;
}

void BlastHornet::SetState(int newState) {
	state = newState;

	switch (newState) {
	case STATE_FLYING:
		SetAnimState(0, 0, ANIM_DELAY);
		break;
	case STATE_PREPARE_CHARGE:
		SetAnimState(1, 12, ANIM_DELAY);
		break;
	case STATE_CHARGE:
		SetAnimState(6, 6, ANIM_DELAY);
		break;
	case STATE_PREPARE_SHOOT:
		SetAnimState(13, 16, ANIM_DELAY);
		break;
	case STATE_SHOOT:
		SetAnimState(17, 18, ANIM_DELAY);
		break;
	}
}

void BlastHornet::OnCollision(MObject *otherObj, char* sideCollided) {

}

void BlastHornet::Update() {
	if (phase == 1) {
		if (state == STATE_FLYING) {

		}
	}
}

void BlastHornet::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	x += movex;
	y += movey;
	anim->Animate(matrix);
}