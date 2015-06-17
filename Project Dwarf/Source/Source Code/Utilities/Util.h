#pragma once
#include "../Application/stdafx.h"

//Returns a random value between -1.0 and +1.0
float SignedRandFloat();

//Returns a random value between 0.0 and +1.0
float UnsignedRandFloat();



using namespace DirectX;

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