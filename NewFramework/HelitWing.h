#pragma once

#include "MObject.h"

#define ANIM_DELAY 0

#define OFFSET_X 2
#define OFFSET_Y -39

class HelitWing : public MObject
{
	MObject* helit;
public:
	HelitWing();
	HelitWing(MObject* _helit);
	~HelitWing();

	void Update();
	void Render();
};

