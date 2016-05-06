#include "BufferDefines.h"

struct tPoint_Light
{
	float4	fLight_Pos;
	float3	fLight_Col;
	float	fRadius;
	uint	uActive;

//#if __cplusplus
//	tPoint_Light();
//	~tPoint_Light();
//	tPoint_Light(tPoint_Light &cpy);
//	tPoint_Light& operator=(tPoint_Light &assign);
//#endif
};

struct tCone_Light
{
	float4	fLight_Pos;
	float3	fLight_Dir;
	float	fRadius;
	float3	fLight_Col;
	uint	uActive;
	float2	fCone_Ratios;

	//#if __cplusplus
	//	tCone_Light();
	//	~tCone_Light();
	//	tCone_Light(tCone_Light &cpy);
	//	tCone_Light& operator=(tCone_Light &assign);
	//#endif
};

struct tDirectional_Light
{
	float4	fLight_Pos;
	float3	fLight_Dir;
	uint	uActive;
	float3	fLight_Col;

//#if __cplusplus
//	tDirectional_Light();
//	~tDirectional_Light();
//	tDirectional_Light(tDirectional_Light &cpy);
//	tDirectional_Light& operator=(tDirectional_Light &assign);
//#endif
};

struct tAmbient_Light
{
	float3 fLight_Col;

};

