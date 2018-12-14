#include "Effects.h"


MObject* Effects::megamanEnergy = NULL;

Effects::Effects()
{
}


Effects::~Effects()
{
	delete megamanEnergy;
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
}