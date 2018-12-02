#pragma once

#include "dsutil.h"
#include"dxutil.h"
#include<d3d9.h>
#include<d3dx9.h>


class DXSound
{	
public:
	//primary DirectSound Object
	//create DirectSound manager object
	CSoundManager *dsound;
	//function prototypes
	DXSound();
	int Init_DirectSound(HWND);
	CSound *LoadSound(char *);
	void PlaySound(CSound *);
	void LoopSound(CSound *);
	void StopSound(CSound *);
};
