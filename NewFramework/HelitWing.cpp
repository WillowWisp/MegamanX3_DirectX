#include "HelitWing.h"

HelitWing::HelitWing() {

}

HelitWing::HelitWing(MObject* _helit)
{
	helit = _helit;
	dirRight = helit->dirRight;
	x = helit->x + OFFSET_X * dirRight;
	y = helit->y + OFFSET_Y;

	anim = new Animation(5, 0, 4, ANIM_DELAY);
	char s[50];
	for (int i = 0; i < 5; i++) {
		sprintf_s(s, "sprites/helit/wing/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
}


HelitWing::~HelitWing()
{
}

void HelitWing::Update() {
	movex = helit->movex;
	movey = helit->movey;
}

void HelitWing::Render() {
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