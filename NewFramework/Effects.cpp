#include "Effects.h"


MObject* Effects::megamanEnergy = NULL;
std::vector<MObject*> Effects::explosionList;
std::vector<MObject*> Effects::smokeList;
std::vector<MObject*> Effects::megamanDeathEnergyList;
Sprite* Effects::screenFilter;

Effects::Effects()
{
}


Effects::~Effects()
{
	delete megamanEnergy;
	for (int i = 0; i < explosionList.size(); i++) {
		delete explosionList[i];
	}
	for (int i = 0; i < smokeList.size(); i++) {
		delete smokeList[i];
	}
	for (int i = 0; i < megamanDeathEnergyList.size(); i++) {
		delete megamanDeathEnergyList[i];
	}
	delete screenFilter;
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

void Effects::ResetMegamanEnergy() {
	megamanEnergy->anim->ChangeAnimFrames(0, 10);
	megamanEnergy->state = 0;
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

void Effects::CreateMegamanDeathEnergy(int x, int y, int movex, int movey) {
	MObject* deathEnergy = new MObject();
	deathEnergy->anim = new Animation(6, 0, 5, 1);
	char s[50];
	for (int i = 0; i < 6; i++) {
		sprintf_s(s, "sprites/megaman_effects/death/%d.png", i);
		deathEnergy->anim->sprite[i] = new Sprite(s);
	}
	deathEnergy->x = x;
	deathEnergy->y = y;
	deathEnergy->movex = movex;
	deathEnergy->movey = movey;
	deathEnergy->delta_t = 0;
	megamanDeathEnergyList.push_back(deathEnergy);
}

void Effects::CreateMegamanDeathEffect(int x, int y) {
	CreateMegamanDeathEnergy(x, y, 0, -DEATH_ENERGY_SPEED);
	CreateMegamanDeathEnergy(x, y, 0, DEATH_ENERGY_SPEED);
	CreateMegamanDeathEnergy(x, y, DEATH_ENERGY_SPEED, 0);
	CreateMegamanDeathEnergy(x, y, -DEATH_ENERGY_SPEED, 0);
	CreateMegamanDeathEnergy(x, y, -DEATH_ENERGY_SPEED / SQUAREROOT_2, -DEATH_ENERGY_SPEED / SQUAREROOT_2);
	CreateMegamanDeathEnergy(x, y, -DEATH_ENERGY_SPEED / SQUAREROOT_2, DEATH_ENERGY_SPEED / SQUAREROOT_2);
	CreateMegamanDeathEnergy(x, y, DEATH_ENERGY_SPEED / SQUAREROOT_2, -DEATH_ENERGY_SPEED / SQUAREROOT_2);
	CreateMegamanDeathEnergy(x, y, DEATH_ENERGY_SPEED / SQUAREROOT_2, DEATH_ENERGY_SPEED / SQUAREROOT_2);
}

void Effects::DrawMegamanDeathEffect() {
	for (int i = 0; i < megamanDeathEnergyList.size(); i++) {
		if (megamanDeathEnergyList[i]->delta_t > DEATH_ENERGY_EXIST_TIME) {
			delete megamanDeathEnergyList[i];
			megamanDeathEnergyList.erase(megamanDeathEnergyList.begin() + i);
			i--;
		}
		else {
			//Update
			//if (megamanDeathEnergyList[i]->movex > 0) {
			//	megamanDeathEnergyList[i]->movex--;
			//}
			//else {
			//	megamanDeathEnergyList[i]->movex++;
			//}
			//if (megamanDeathEnergyList[i]->movey > 0) {
			//	megamanDeathEnergyList[i]->movey--;
			//}
			//else {
			//	megamanDeathEnergyList[i]->movey++;
			//}
			/*megamanDeathEnergyList[i]->movex--;
			megamanDeathEnergyList[i]->movey++;*/

			float sqrt2 = 1.4;

			megamanDeathEnergyList[i]->x += megamanDeathEnergyList[i]->movex;
			megamanDeathEnergyList[i]->y += megamanDeathEnergyList[i]->movey;

			/*if (megamanDeathEnergyList[i]->movey < 0 && megamanDeathEnergyList[i]->movex != 0) {
				megamanDeathEnergyList[i]->x += megamanDeathEnergyList[i]->delta_t;
			}
			else if (megamanDeathEnergyList[i]->movey == 0) {
				megamanDeathEnergyList[i]->x += megamanDeathEnergyList[i]->movex > 0 
												? -megamanDeathEnergyList[i]->delta_t * sqrt2
												: megamanDeathEnergyList[i]->delta_t * sqrt2;
			}
			else if (megamanDeathEnergyList[i]->movex != 0)	{
				megamanDeathEnergyList[i]->x -= megamanDeathEnergyList[i]->delta_t;
			}
			else {
				megamanDeathEnergyList[i]->x += megamanDeathEnergyList[i]->movey > 0
												? -megamanDeathEnergyList[i]->delta_t * sqrt2
												: megamanDeathEnergyList[i]->delta_t * sqrt2;
			}

			if (megamanDeathEnergyList[i]->movex < 0 && megamanDeathEnergyList[i]->movey != 0) {
				megamanDeathEnergyList[i]->y -= megamanDeathEnergyList[i]->delta_t;
			}
			else if (megamanDeathEnergyList[i]->movex == 0) {
				megamanDeathEnergyList[i]->y += megamanDeathEnergyList[i]->movey > 0
												? -megamanDeathEnergyList[i]->delta_t * sqrt2
												: +megamanDeathEnergyList[i]->delta_t * sqrt2;
			}
			else if (megamanDeathEnergyList[i]->movey != 0) {
				megamanDeathEnergyList[i]->y += megamanDeathEnergyList[i]->delta_t;
			}
			else {
				megamanDeathEnergyList[i]->y += megamanDeathEnergyList[i]->movex > 0
												? megamanDeathEnergyList[i]->delta_t * sqrt2
												: -megamanDeathEnergyList[i]->delta_t * sqrt2;
			}*/
			
			megamanDeathEnergyList[i]->delta_t++;

			//Render
			D3DXVECTOR2 translation = D3DXVECTOR2(megamanDeathEnergyList[i]->x, megamanDeathEnergyList[i]->y);
			D3DXVECTOR2 translate = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
			D3DXVECTOR2 combined = translation + translate;

			D3DXVECTOR2 scale = D3DXVECTOR2(1, 1);
			//center = D3DXVECTOR3(width / 2, height / 2, 0);
			D3DXMatrixTransformation2D(&(megamanDeathEnergyList[i]->matrix), NULL, 0, &scale, NULL,
				NULL, &combined);
			megamanDeathEnergyList[i]->Render();
		}
	}
}

void Effects::CreateExplosion(int x, int y) {
	MObject* explosion = new MObject();
	explosion->anim = new Animation(8, 0, 7, 1);
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

void Effects::CreateSmoke(int x, int y, int movey) {
	MObject* smoke = new MObject();
	smoke->anim = new Animation(6, 0, 5, 1);
	char s[50];
	for (int i = 0; i < 6; i++) {
		sprintf_s(s, "sprites/effects/smoke/%d.png", i);
		smoke->anim->sprite[i] = new Sprite(s);
	}
	smoke->x = x;
	smoke->y = y;
	smoke->movey = movey;
	smokeList.push_back(smoke);
}

void Effects::DrawSmokes() {
	for (int i = 0; i < smokeList.size(); i++) {
		if (smokeList[i]->anim->curframe == smokeList[i]->anim->endframe
			&& smokeList[i]->anim->animcount > smokeList[i]->anim->animdelay) {
			delete smokeList[i];
			smokeList.erase(smokeList.begin() + i);
			i--;
		}
		else {
			smokeList[i]->y += smokeList[i]->movey;
			D3DXVECTOR2 translation = D3DXVECTOR2(smokeList[i]->x, smokeList[i]->y);
			D3DXVECTOR2 translate = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
			D3DXVECTOR2 combined = translation + translate;

			D3DXVECTOR2 scale = D3DXVECTOR2(2, 2);
			//center = D3DXVECTOR3(width / 2, height / 2, 0);
			D3DXMatrixTransformation2D(&(smokeList[i]->matrix), NULL, 0, &scale, NULL,
				NULL, &combined);
			smokeList[i]->Render();
		}
	}
}

void Effects::RenderEffects() {
	if (explosionList.size() > 0) {
		Effects::DrawExplosions();
		/*GAMELOG("explosions: %d", explosionList.size());*/
	}
	if (smokeList.size() > 0) {
		Effects::DrawSmokes();
	}
	if (megamanDeathEnergyList.size() > 0) {
		Effects::DrawMegamanDeathEffect();
	}
}

void Effects::CreateScreenFilter() {
	char s[50];
	sprintf_s(s, "sprites/effects/screen_filter/white.png");
	screenFilter = new Sprite(s);
}

void Effects::DrawScreenFilter(D3DCOLOR color) {
	D3DXMATRIX matrix;

	D3DXVECTOR2 translation = D3DXVECTOR2(GameGlobal::camera->width / 2, GameGlobal::camera->height / 2);

	D3DXVECTOR2 scale = D3DXVECTOR2(2, 2);
	//center = D3DXVECTOR3(width / 2, height / 2, 0);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &translation);
	screenFilter->Draw(matrix, color);
}