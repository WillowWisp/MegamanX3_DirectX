#pragma once

#include "dsutil.h"
#include"dxutil.h"
#include<d3d9.h>
#include<d3dx9.h>

class Sound
{
public:
	Sound();
	~Sound();
	//function prototypes
	static int Init_DirectSound(HWND);
	static CSound *LoadSound(char *);
	static void PlaySound(CSound *);
	static void LoopSound(CSound *);
	static void StopSound(CSound *);
};