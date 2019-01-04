#pragma once
#include <stdlib.h>
#include <time.h>

class Random
{
public:
	//static 
	Random();
	~Random();
	static void Init();
	static int RandInt(int low, int high);
};

