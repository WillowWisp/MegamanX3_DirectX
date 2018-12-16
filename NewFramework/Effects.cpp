#include "Effects.h"


MObject* Effects::megamanEnergy = NULL;
std::vector<MObject*> Effects::explosionList;

Effects::Effects()
{
}


Effects::~Effects()
{
	delete megamanEnergy;
	for (int i = 0; i < explosionList.size(); i++) {
		delete explosionList[i];
	}
}

void Effects::CreateMegamanEnergy() {
	Effects::megamanEnergy = new MObject();
	Effects::megamanEnergy->anim = new Animation(22, 0, 10, -1);
	char s[50];
	for (int i = 0; i < 22; i++) {
		sprintf_s(s, "sprites/megaman_effects/charged/%d.png", i);
		Effects::megamanEnergy->anim->sprite[i] = new Sprite(s, RECT(), NULL, NULL, D3DCOLOR_XRGB(64, 102, 232));
	}
}

void Effects::DrawMegamanEnergy(D3DXMATRIX matrix, int x, int y, int level) {
	if (level == 2 && Effects::megamanEnergy->state == 0) {
		Effects::megamanEnergy->anim->ChangeAnimFrames(11, 21);
		Effects::megamanEnergy->state = 1;
	}

	Effects::megamanEnergy->matrix = matrix;
	Effects::megamanEnergy->x = x;
	Effects::megamanEnergy->y = y;

	Effects::megamanEnergy->Update();
	Effects::megamanEnergy->Render();
}

void Effects::CreateExplosion(int x, int y) {
	MObject* explosion = new MObject();
	explosion->anim = new Animation(8, 0, 7, 0);
	char s[50];
	for (int i = 0; i < 8; i++) {
		sprintf_s(s, "sprites/effects/explosion/%d.png", i);
		explosion->anim->sprite[i] = new Sprite(s);
	}
	explosion->x = x;
	explosion->y = y;
	explosionList.push_back(explosion);
}

void Effects::DrawExplosions() {
	for (int i = 0; i < explosionList.size(); i++) {
		if (explosionList[i]->anim->curframe == explosionList[i]->anim->endframe
			&& explosionList[i]->anim->animcount > explosionList[i]->anim->animdelay) {
			delete explosionList[i];
			explosionList.erase(explosionList.begin() + i);
			i--;
		}
		else {
			D3DXVECTOR2 translation = D3DXVECTOR2(explosionList[i]->x, explosionList[i]->y);
			D3DXVECTOR2 translate = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
			D3DXVECTOR2 combined = translation + translate;

			D3DXVECTOR2 scale = D3DXVECTOR2(2, 2);
			//center = D3DXVECTOR3(width / 2, height / 2, 0);
			D3DXMatrixTransformation2D(&(explosionList[i]->matrix), NULL, 0, &scale, NULL,
				NULL, &combined);
			explosionList[i]->Render();
		}
	}
}

void Effects::RenderEffects() {
	if (explosionList.size() > 0) {
		Effects::DrawExplosions();
		/*GAMELOG("explosions: %d", explosionList.size());*/
	}
}