#include "BufferDefines.h"

///Vertex buffers input
struct v_StaticNoTangs{
	float3 pos	SEMANTIC(POSITION);
	float3 norm SEMANTIC(NORMAL);
	float2 uv	SEMANTIC(TEXCOORD);
};

struct v_StaticWithTangs{
	float3 pos	SEMANTIC(POSITION);
	float3 tang	SEMANTIC(TANGENT);
	float3 norm	SEMANTIC(NORMAL);
	float2 uv	SEMANTIC(TEXCOORD);
};

struct v_AnimNoTangents{
	float3 pos		SEMANTIC(POSITION);
	float3 norm		SEMANTIC(NORMAL);
	float2 uv		SEMANTIC(TEXCOORD);
	uint4 joint_inf	SEMANTIC(JOINT_INF);
	float4 weights	SEMANTIC(WEIGHTS);
};

struct v_AnimWithTangs{
	float3 pos		SEMANTIC(POSITION);
	float3 tang		SEMANTIC(TANGENT);
	float3 norm		SEMANTIC(NORMAL);
	float2 uv		SEMANTIC(TEXCOORD);
	uint4 joint_inf	SEMANTIC(JOINT_INF);
	float4 weights	SEMANTIC(WEIGHTS);
};

///Vertex buffers output
struct v_StaticOut{
	float4 pos		SEMANTIC(SV_POSITION);
	float3 norm		SEMANTIC(NORMAL);
	float2 uv		SEMANTIC(TEXCOORD);
	float4 color	SEMANTIC(COLOR);
};