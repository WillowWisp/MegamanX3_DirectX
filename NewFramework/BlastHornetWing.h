#pragma once

#include "MObject.h"

#define ANIM_DELAY -1

#define OFFSET_X 8
#define OFFSET_Y -75

class BlastHornetWing : public MObject
{
	MObject* blastHornet;
public:
	BlastHornetWing();
	BlastHornetWing(MObject* _blastHornet);
	~BlastHornetWing();

	void Update();
	void Render();
};