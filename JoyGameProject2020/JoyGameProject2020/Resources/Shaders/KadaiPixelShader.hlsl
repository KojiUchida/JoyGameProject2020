#include "KadaiShaderHeader.hlsli"

float4 PSmain(GSOutput input) : SV_TARGET {
	return tex.Sample(smp, input.uv);
}