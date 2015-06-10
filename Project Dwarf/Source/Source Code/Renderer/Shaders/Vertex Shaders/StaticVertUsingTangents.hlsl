#include "../VertexBuffers.h"
#include "../ConstantBuffers.h"
#include "../CommonFunctions.hlsli"

v_StaticOut main( v_StaticWithTangs v_in )
{
	v_StaticOut v_out = (v_StaticOut)0;

	v_out.pos = float4(v_in.pos, 1.0f);
	v_out.norm = v_in.norm;
	v_out.uv = v_in.uv;

	return v_out;
}