#include "../Application/stdafx.h"

#include "Util.h"

//Returns a random value between -1.0 and +1.0
float SignedRandFloat()
{
	int rand_value = (rand() % 200001) - 100000;

	return (float(rand_value) / 100000);
}

//Returns a random value between 0.0 and +1.0
float UnsignedRandFloat()
{
	int rand_value = rand() % 100001;

	return (float(rand_value) / 100000);
}