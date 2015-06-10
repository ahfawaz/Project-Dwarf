#include "BufferDefines.h"

struct PixelInput{
	float4 pos		SEMANTIC(SV_POSITION);
	float3 norm		SEMANTIC(NORMAL);
	float2 uv		SEMANTIC(TEXCOORD);
	float4 color	SEMANTIC(COLOR);
};