#include "Random.h"



Random::Random()
{
}


Random::~Random()
{
}

void Random::Init() {
	srand(time(NULL));
}

int Random::RandInt(int low, int high) {
	return rand() % (high - low) + low;
}