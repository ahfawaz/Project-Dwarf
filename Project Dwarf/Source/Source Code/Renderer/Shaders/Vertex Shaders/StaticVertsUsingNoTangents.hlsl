#include "../VertexBuffers.h"
#include "../ConstantBuffers.h"
#include "../CommonFunctions.hlsli"



v_StaticOut main(v_StaticNoTangs v_in)
{
	v_StaticOut v_out = (v_StaticOut)0;

	float4 pos = float4(v_in.pos, 1.0f);
		pos = mul(pos, world_matrix);
	pos = mul(pos, view_matrix);
	pos = mul(pos, proj_matrix);

	float3 norm = v_in.norm;
		float4 color_out = float4((norm.x + 1.0f) * 0.5f, (norm.y + 1.0f) * 0.5f, (norm.z + 1.0f) * 0.5f, 1.0f);

	v_out.pos = pos;
	v_out.norm = norm;
	v_out.uv = v_in.uv;
	v_out.color = color_out;

	return v_out;
}