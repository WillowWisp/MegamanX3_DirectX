#include "ByteMine.h"



ByteMine::ByteMine()
{
}


ByteMine::~ByteMine()
{
}

ByteMine::ByteMine(int _x, int _y, int _dirRight) {
	x = _x;
	y = _y;
	dirRight = _dirRight;
	if (dirRight > 0) {
		destX = GameGlobal::camera->GetBound().right - 40;
	}
	else {
		destX = GameGlobal::camera->GetBound().left + 40;
	}
	//destX = _destX;
	movex = MINE_SPEED;

	anim = new Animation(6, 0, 2, 1);
	char s[50];
	for (int i = 0; i < 6; i++) {
		sprintf_s(s, "sprites/byte/mine/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}
}

void ByteMine::Explode() {
	Effects::CreateExplosion(x, y);
	//delete this;
}

void ByteMine::Update() {
	if (anim->curframe >= 0 && anim->curframe <= 2) {	//flying
		if ((x - destX) * dirRight > 0) {	//touch wall
			anim->ChangeAnimFrames(3, 5);
			movex = 0;
		}
	}
	else if (anim->curframe == 5) {		//done animation
		anim->animcount = 0;
	}
	MObject::Update();
}

void ByteMine::Render() {
	D3DXVECTOR2 translation = D3DXVECTOR2(x + movex * dirRight, y + movey);
	D3DXVECTOR2 shift = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
	D3DXVECTOR2 combined = translation + shift;

	D3DXVECTOR2 scale = D3DXVECTOR2(-2 * dirRight, 2);
	//center = D3DXVECTOR3(width / 2, height / 2, 0);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &combined);

	MObject::Render();
}