#include "FbxShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD) {
	VSOutput output;
	output.svpos = mul(mvp, pos);
	output.normal = mul(rot, normal);
	output.uv = uv;
	return output;
}