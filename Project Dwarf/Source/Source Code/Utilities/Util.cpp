#include "../Application/stdafx.h"

#include "Util.h"

namespace Dwarf_Util
{
	//Returns a random value between -1.0 and +1.0
	float SignedRandFloat(float _hi, float _low)
	{
		return _low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_hi - (_low))));
	}

	//Returns a random value between 0.0 and +1.0
	float UnsignedRandFloat()
	{
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}

	XMFLOAT3 RandomColor()
	{
		return{ UnsignedRandFloat(), UnsignedRandFloat(), UnsignedRandFloat() };
	}

	XMFLOAT3 RandomColorConstrained(float _red_min, float _red_max, float _green_min, float _green_max, float _blue_min, float _blue_max)
	{
		return{ SignedRandFloat(_red_max, _red_min), SignedRandFloat(_green_max, _green_min), SignedRandFloat(_blue_max, _blue_min) };
	}

	XMFLOAT3 RandomColorConstrained(XMFLOAT3 _min_colors, XMFLOAT3 _max_colors)
	{
		return RandomColorConstrained(_min_colors.x, _max_colors.x, _min_colors.y, _max_colors.y, _min_colors.z, _max_colors.z);
	}

	XMFLOAT3 RandomColorConstrained(float _min_colors, float _max_colors)
	{
		return RandomColorConstrained(_min_colors, _max_colors, _min_colors, _max_colors, _min_colors, _max_colors);

	}

}