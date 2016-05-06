#pragma once
#include "../Application/stdafx.h"

namespace Dwarf_Util
{
	using namespace DirectX;


	//Returns a random value between -1.0 and +1.0
	float SignedRandFloat(float _hi = -1.0f, float _low = 1.0f);

	//Returns a random value between 0.0 and +1.0
	float UnsignedRandFloat();

	XMFLOAT3 RandomColor();
	XMFLOAT3 RandomColorConstrained(float _red_min, float _red_max, float _green_min, float _green_max, float _blue_min, float _blue_max);
	XMFLOAT3 RandomColorConstrained(XMFLOAT3 _min_colors, XMFLOAT3 _max_colors);
	XMFLOAT3 RandomColorConstrained(float _min_colors, float _max_colors);


#if defined(DEBUG) | defined(_DEBUG)

#pragma comment( lib, "dxguid.lib")

#ifndef SetD3DName
#define SetD3DName(object, name)							\
		{															\
		if(name)												\
			object->SetPrivateData(WKPDID_D3DDebugObjectName,	\
				(UINT)strlen( name ), name );					\
		}
#endif
#else
#ifndef SetD3DName
#define SetD3DName(object, name) 
#endif
#endif 
}