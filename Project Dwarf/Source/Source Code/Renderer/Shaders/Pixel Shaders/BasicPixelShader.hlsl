#include "../PixelBuffers.h"
#include "../ConstantBuffers.h"
#include "../CommonFunctions.hlsli"

float4 main(PixelInput v_input) : SV_TARGET
{
	return v_input.color;
}