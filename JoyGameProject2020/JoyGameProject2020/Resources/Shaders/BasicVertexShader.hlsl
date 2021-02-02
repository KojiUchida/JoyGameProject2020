#include "BasicShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD) {
	VSOutput output;
	output.svpos = mul(mvp, pos);
	output.worldpos = mul(world, pos);
	output.normal = normalize(mul(world, normal));
	output.uv = uv;
	return output;
}