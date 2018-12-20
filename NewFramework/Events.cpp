#include "Events.h"

std::vector<MObject*> Events::doorsList; 
bool Events::isOpeningDoor = false;
bool Events::isFightingBoss = false;
int Events::openingDoorId = -1;
Megaman* Events::megaman;

Events::Events()
{
}


Events::~Events()
{
	for (int i = 0; i < doorsList.size(); i++) {
		delete doorsList[i];
	}
}

void Events::CreateDoor(int x, int y) {
	MObject* door = new MObject();
	door->x = x;
	door->y = y;
	
	door->anim = new Animation(35, 0, 0, 1);
	char s[50];
	for (int i = 0; i < 35; i++) {
		sprintf_s(s, "sprites/door/%d.png", i);
		door->anim->sprite[i] = new Sprite(s);
	}

	doorsList.push_back(door);
}

void Events::OpenDoor(int doorId) {
	if (doorsList[doorId]->anim->endframe == 0) {		//Start opening
		doorsList[doorId]->anim->ChangeAnimFrames(1, 17);
		isOpeningDoor = true;
		openingDoorId = doorId;
		megaman->isControllable = false;
		megaman->movex = 0;
		megaman->movey = 0;
	}
	else if (doorsList[doorId]->anim->curframe == 17) {
		//force megaman move
		if (megaman->x - megaman->width > doorsList[doorId]->x + doorsList[doorId]->width / 2) {
			doorsList[doorId]->anim->ChangeAnimFrames(18, 34);
			doorsList[doorId]->anim->curframe = 18;
			doorsList[doorId]->anim->animcount = 0;
			megaman->movex = 0;
			//megaman->movey = 0;

		}
		else {
			//doorsList[doorId]->anim->curframe = 17;
			doorsList[doorId]->anim->animcount = 0;
			megaman->movex = 3;
			//megaman->movey = 0;
			if (megaman->state != STATE_RUNNING && megaman->state != STATE_RUNNING_N_SHOOTING) {
				megaman->anim->animcount = 0;
			}
		}
	}
	else if (doorsList[doorId]->anim->curframe == 34) {
		doorsList[doorId]->anim->ChangeAnimFrames(34, 34);
		doorsList[doorId]->anim->curframe = 34;
		isOpeningDoor = false;
		//openingDoorId = -1;
		megaman->isControllable = true;
		//isFightingBoss = true;
	}
	else {
		megaman->anim->animcount = 0;
	}

}

void Events::RenderDoors() {
	for (int i = 0; i < doorsList.size(); i++) {
		D3DXVECTOR2 translation = D3DXVECTOR2(doorsList[i]->x, doorsList[i]->y);
		D3DXVECTOR2 translate = D3DXVECTOR2(GameGlobal::wndWidth / 2 - GameGlobal::camera->position.x, GameGlobal::wndHeight / 2 - GameGlobal::camera->position.y);
		D3DXVECTOR2 combined = translation + translate;

		D3DXVECTOR2 scale = D3DXVECTOR2(2, 2);
		//center = D3DXVECTOR3(width / 2, height / 2, 0);
		D3DXMatrixTransformation2D(&(doorsList[i]->matrix), NULL, 0, &scale, NULL,
			NULL, &combined);
		doorsList[i]->Render();
	}
}

void Events::RenderEvents() {
	RenderDoors();
}