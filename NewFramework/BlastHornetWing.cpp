#include "BlastHornetWing.h"
#include "GameLog.h"

BlastHornetWing::BlastHornetWing() {

}

BlastHornetWing::BlastHornetWing(MObject* _blastHornet)
{
	blastHornet = _blastHornet;
	dirRight = blastHornet->dirRight;
	x = blastHornet->x + OFFSET_X * dirRight;
	y = blastHornet->y + OFFSET_Y;

	anim = new Animation(4, 0, 3, ANIM_DELAY);
	char s[50];
	for (int i = 0; i < 4; i++) {
		sprintf_s(s, "sprites/blast_hornet/wing/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
}


BlastHornetWing::~BlastHornetWing()
{
}

void BlastHornetWing::Update() {
	x = blastHornet->x + OFFSET_X * dirRight;
	y = blastHornet->y + OFFSET_Y;
}

void BlastHornetWing::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x, y);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(1 * dirRight, 1);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);
	anim->Animate(matrix);
}